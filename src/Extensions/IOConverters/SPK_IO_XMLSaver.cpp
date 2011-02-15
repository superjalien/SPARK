//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2011 - Julien Fryer - julienfryer@gmail.com				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#include <ctime>

#define TIXML_USE_STL
#include <tinyxml.h>

#include <SPARK_Core.h>
#include "Extensions/IOConverters/SPK_IO_XMLSaver.h"

namespace SPK
{
namespace IO
{
	bool XMLSaver::innerSave(std::ostream& os,Graph& graph) const
	{
		TiXmlDocument doc;

		// Header
		doc.LinkEndChild(new TiXmlDeclaration("1.0","",""));
		time_t currentTime = time(NULL);
		tm* timeinfo = localtime(&currentTime);
		std::string headerComment(" File automatically generated by SPARK on ");
		headerComment += asctime(timeinfo);
		headerComment.replace(headerComment.size() - 1,1,1,' '); // replace the '\n' generated by a space
		doc.LinkEndChild(new TiXmlComment(headerComment.c_str()));
		if (!author.empty())
		{
			std::string authorStr(" Author : " + author + " "); 
			doc.LinkEndChild(new TiXmlComment(authorStr.c_str()));
		}

		// Root
		TiXmlElement* root = new TiXmlElement("SPARK");
		doc.LinkEndChild(root);

		Node* node = NULL;
		while ((node = graph.getNextNode()) != NULL)
			if (!node->isProcessed())
				if (!writeNode(*root,*node,graph))
					return false;

		TiXmlPrinter printer;
		printer.SetIndent(layout.indent.c_str());
		printer.SetLineBreak(layout.lineBreak.c_str());
		doc.Accept(&printer);
		os << printer.Str();
		return true;
	}

	void XMLSaver::writeValue(TiXmlElement& attrib,const std::string& value) const
	{
		switch(layout.valueLayout)
		{
		case XML_VALUE_LAYOUT_AS_ATTRIBUTE : attrib.SetAttribute("value",value); break;
		case XML_VALUE_LAYOUT_AS_TEXT : attrib.LinkEndChild(new TiXmlText(value)); break;
		}
	}

	void XMLSaver::writeRef(TiXmlElement& attrib,const Node& node) const
	{
		std::string refStr(format(node.getReferenceID()));
		switch(layout.valueLayout)
		{
		case XML_VALUE_LAYOUT_AS_ATTRIBUTE : attrib.SetAttribute("ref",refStr); break;
		case XML_VALUE_LAYOUT_AS_TEXT : attrib.LinkEndChild(new TiXmlText(refStr)); break;
		}
	}

	bool XMLSaver::writeObject(TiXmlElement& parent,const Ref<SPKObject>& object,Graph& graph,bool refInTag) const
	{
		Node* refNode = graph.getNode(object);
		if (refNode == NULL)
		{
			SPK_LOG_ERROR("XML ERROR 1");
			return false;
		}
		if ((layout.referenceRule == XML_REFERENCE_RULE_WHEN_NEEDED && refNode->getNbReferences() > 1) 
			|| layout.referenceRule == XML_REFERENCE_RULE_FORCED
			|| (layout.referenceRule == XML_REFERENCE_RULE_DESCRIBED_AT_FIRST && refNode->isProcessed()))
		{
			if (refInTag)
			{
				TiXmlElement* ref = new TiXmlElement("Ref");
				writeRef(*ref,*refNode);
				parent.LinkEndChild(ref);

			}
			else
				writeRef(parent,*refNode);
			return true;
		}
		else 
			return writeNode(parent,*refNode,graph);
	}

	bool XMLSaver::writeNode(TiXmlElement& parent,const Node& node,Graph& graph) const
	{
		const Descriptor& desc = node.getDescriptor();

		TiXmlElement* element = new TiXmlElement(desc.getName());
		parent.LinkEndChild(element);

		for (size_t i = 0; i < desc.getNbAttributes(); ++i)
		{
			const Attribute& attrib = desc.getAttribute(i);
			if (attrib.hasValue() && !attrib.isValueOptional())
			{
				if (attrib.getName() == "name")
					element->SetAttribute("name",attrib.getValue<std::string>());
				else
				{
					TiXmlElement* child = new TiXmlElement("attrib");
					element->LinkEndChild(child);					
					child->SetAttribute("id",attrib.getName());
					
					switch (attrib.getType())
					{
					case ATTRIBUTE_TYPE_CHAR :		writeValue(*child,format(attrib.getValue<char>())); break;
					case ATTRIBUTE_TYPE_BOOL :		writeValue(*child,format(attrib.getValue<bool>())); break;
					case ATTRIBUTE_TYPE_INT32 :		writeValue(*child,format(attrib.getValue<long>())); break;
					case ATTRIBUTE_TYPE_UINT32 :	writeValue(*child,format(attrib.getValue<unsigned long>())); break;
					case ATTRIBUTE_TYPE_FLOAT :		writeValue(*child,format(attrib.getValue<float>())); break;
					case ATTRIBUTE_TYPE_VECTOR :	writeValue(*child,format(attrib.getValue<Vector3D>())); break;
					case ATTRIBUTE_TYPE_COLOR :		writeValue(*child,format(attrib.getValue<Color>())); break;
					case ATTRIBUTE_TYPE_STRING :	writeValue(*child,format(attrib.getValue<std::string>())); break;	

					case ATTRIBUTE_TYPE_CHARS :		writeValue(*child,formatArray(attrib.getValues<char>())); break;
					case ATTRIBUTE_TYPE_BOOLS :		writeValue(*child,formatArray(attrib.getValues<bool>())); break;
					case ATTRIBUTE_TYPE_INT32S :	writeValue(*child,formatArray(attrib.getValues<long>())); break;
					case ATTRIBUTE_TYPE_UINT32S :	writeValue(*child,formatArray(attrib.getValues<unsigned long>())); break;
					case ATTRIBUTE_TYPE_FLOATS :	writeValue(*child,formatArray(attrib.getValues<float>())); break;
					case ATTRIBUTE_TYPE_VECTORS :	writeValue(*child,formatArray(attrib.getValues<Vector3D>())); break;
					case ATTRIBUTE_TYPE_COLORS :	writeValue(*child,formatArray(attrib.getValues<Color>())); break;
					case ATTRIBUTE_TYPE_STRINGS :	writeValue(*child,formatArray(attrib.getValues<std::string>())); break;

					case ATTRIBUTE_TYPE_REF :		
						if (!writeObject(*child,attrib.getValueRef<SPKObject>(),graph,false)) 
							return false; 
						break;

					case ATTRIBUTE_TYPE_REFS : {
						const std::vector<Ref<SPKObject>>& refs = attrib.getValuesRef<SPKObject>();
						for (size_t i = 0; i < refs.size(); ++i)
							if (!writeObject(*child,refs[i],graph,true)) 
								return false; 
						break; }

					default :
						SPK_LOG_ERROR("XML ERROR 3");
					}
				}
			}
		}

		if (node.getNbReferences() > 1 || layout.referenceRule == XML_REFERENCE_RULE_FORCED)
			element->SetAttribute("ref",format(node.getReferenceID()));	
		node.markAsProcessed();
		return true;
	}
}}