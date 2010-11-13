//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2010 - Julien Fryer - julienfryer@gmail.com				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to  anyone to use this software for any purpose,		//
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

#ifndef H_SPK_DESCRIPTOR
#define H_SPK_DESCRIPTOR

#include <vector>
#include <string>

namespace SPK
{
	class Descriptor;
	class SPKObject;

	enum AttributeType
	{
		ATTRIBUTE_TYPE_CHAR,
		ATTRIBUTE_TYPE_BOOL,
		ATTRIBUTE_TYPE_INT32,
		ATTRIBUTE_TYPE_UINT32,
		ATTRIBUTE_TYPE_FLOAT,
		ATTRIBUTE_TYPE_VECTOR,
		ATTRIBUTE_TYPE_COLOR,
		ATTRIBUTE_TYPE_STRING,
		ATTRIBUTE_TYPE_REF,
		ATTRIBUTE_TYPE_CHARS,
		ATTRIBUTE_TYPE_BOOLS,
		ATTRIBUTE_TYPE_INT32S,
		ATTRIBUTE_TYPE_UINT32S,
		ATTRIBUTE_TYPE_FLOATS,
		ATTRIBUTE_TYPE_VECTORS,
		ATTRIBUTE_TYPE_COLORS,
		ATTRIBUTE_TYPE_STRINGS,
		ATTRIBUTE_TYPE_REFS,
	};

	class SPK_PREFIX Attribute
	{
	friend class Descriptor;

	public : 

		Attribute(const std::string& name,AttributeType type);

		inline const std::string& getName() const;
		inline AttributeType getType() const;
		inline bool hasValue() const;
		inline bool isValueOptional() const;

		// For type safety
		void setValueChar(char value,bool optional = false)										{ setValue(ATTRIBUTE_TYPE_CHAR,value,optional); }
		void setValueBool(bool value,bool optional = false)										{ setValue(ATTRIBUTE_TYPE_BOOL,value,optional); }
		void setValueInt32(long int value,bool optional = false)								{ setValue(ATTRIBUTE_TYPE_INT32,value,optional); }
		void setValueUint32(unsigned long int value,bool optional = false)						{ setValue(ATTRIBUTE_TYPE_UINT32,value,optional); }
		void setValueFloat(float value,bool optional = false)									{ setValue(ATTRIBUTE_TYPE_FLOAT,value,optional); }
		void setValueVector(const Vector3D& value,bool optional = false)						{ setValue(ATTRIBUTE_TYPE_VECTOR,value,optional); }
		void setValueColor(const Color& value,bool optional = false)							{ setValue(ATTRIBUTE_TYPE_COLOR,value,optional); }
		void setValueString(const std::string& value,bool optional = false)						{ setValue(ATTRIBUTE_TYPE_STRING,value,optional); }
		void setValueRef(const WeakRef<SPKObject>& value,bool optional = false)					{ setValue(ATTRIBUTE_TYPE_REF,value,optional); }

		void setValuesChar(const char* values,size_t nb,bool optional = false)					{ setValues(ATTRIBUTE_TYPE_CHARS,values,nb,optional); }
		void setValuesBool(const bool* values,size_t nb,bool optional = false)					{ setValues(ATTRIBUTE_TYPE_BOOLS,values,nb,optional); }
		void setValuesInt32(const long int* values,size_t nb,bool optional = false)				{ setValues(ATTRIBUTE_TYPE_INT32S,values,nb,optional); }
		void setValuesUint32(const unsigned long int* values,size_t nb,bool optional = false)	{ setValues(ATTRIBUTE_TYPE_UINT32S,values,nb,optional); }
		void setValuesFloat(const float* values,size_t nb,bool optional = false)				{ setValues(ATTRIBUTE_TYPE_FLOATS,values,nb,optional); }
		void setValuesVector(const Vector3D* values,size_t nb,bool optional = false)			{ setValues(ATTRIBUTE_TYPE_VECTORS,values,nb,optional); }
		void setValuesColor(const Color* values,size_t nb,bool optional = false)				{ setValues(ATTRIBUTE_TYPE_COLORS,values,nb,optional); }
		void setValuesString(const std::string* values,size_t nb,bool optional = false)			{ setValues(ATTRIBUTE_TYPE_STRINGS,values,nb,optional); }
		void setValuesRef(const WeakRef<SPKObject>* values,size_t nb,bool optional = false)		{ setValues(ATTRIBUTE_TYPE_REFS,values,nb,optional); }
		
		char getValueChar() const									{ return getValue<char>(ATTRIBUTE_TYPE_CHAR); }
		bool getValueBool() const									{ return getValue<bool>(ATTRIBUTE_TYPE_BOOL); }
		long int getValueInt32() const								{ return getValue<long int>(ATTRIBUTE_TYPE_INT32); }
		unsigned long int getValueUint32() const					{ return getValue<unsigned long int>(ATTRIBUTE_TYPE_UINT32); }
		float getValueFloat() const									{ return getValue<float>(ATTRIBUTE_TYPE_FLOAT); }
		Vector3D getValueVector() const								{ return getValue<Vector3D>(ATTRIBUTE_TYPE_VECTOR); }
		Color getValueColor() const									{ return getValue<Color>(ATTRIBUTE_TYPE_COLOR); }
		std::string getValueString() const							{ return getValue<std::string>(ATTRIBUTE_TYPE_STRING); }
		WeakRef<SPKObject> getValueRef() const						{ return getValue<WeakRef<SPKObject>>(ATTRIBUTE_TYPE_REF); }

		std::vector<char> getValuesChar() const						{ return getValues<char>(ATTRIBUTE_TYPE_CHARS); }
		std::vector<bool> getValuesBool() const						{ return getValues<bool>(ATTRIBUTE_TYPE_BOOLS); }
		std::vector<long int> getValuesInt32() const				{ return getValues<long int>(ATTRIBUTE_TYPE_INT32S); }
		std::vector<unsigned long int> getValuesUint32() const		{ return getValues<unsigned long int>(ATTRIBUTE_TYPE_UINT32S); }
		std::vector<float> getValuesFloat() const					{ return getValues<float>(ATTRIBUTE_TYPE_FLOATS); }
		std::vector<Vector3D> getValuesVector() const				{ return getValues<Vector3D>(ATTRIBUTE_TYPE_VECTORS); }
		std::vector<Color> getValuesColor() const					{ return getValues<Color>(ATTRIBUTE_TYPE_COLORS); }
		std::vector<std::string> getValuesString() const			{ return getValues<std::string>(ATTRIBUTE_TYPE_STRINGS); }
		std::vector<WeakRef<SPKObject>> getValuesRef() const		{ return getValues<WeakRef<SPKObject>>(ATTRIBUTE_TYPE_REFS); }


	private :

		std::string name;
		AttributeType type;
		size_t offset;

		Descriptor* descriptor;
		
		bool valueSet;
		bool optional;

		// Generic setValue(s)/getValue(s) (encapsulated to ensure type safety)
		template<typename T> void setValue(AttributeType valueType,const T& value,bool optional = false);
		template<typename T> void setValues(AttributeType valueType,const T* values,size_t nb,bool optional = false);
		template<typename T> T getValue(AttributeType valueType) const;
		template<typename T> const std::vector<T> getValues(AttributeType valueType) const;
	};

	class SPK_PREFIX Descriptor
	{
	friend class Attribute;
	friend class SPKObject;

	public :

		Attribute* getAttribute(const std::string& name);
		Attribute* getAttributeWithValue(const std::string& name);
		Attribute& getAttribute(size_t index);

		inline const Attribute* getAttribute(const std::string& name) const;
		inline const Attribute* getAttributeWithValue(const std::string& name) const;
		inline const Attribute& getAttribute(size_t index) const;

		inline size_t getNbAttributes() const;
		inline size_t getSignature() const;

	private :

		Descriptor(const std::vector<Attribute> attributes); // Constructor only accessible by SPKObject

		std::vector<Attribute> attributes;		
		std::vector<char> buffer; // An internal buffer is used to limit memory allocation of attribute values

		unsigned long int signature;

		void computeSignature();
		void markAttributes();
	};

	inline const std::string& Attribute::getName() const
	{
		return name;
	}

	inline AttributeType Attribute::getType() const
	{
		return type;
	}

	inline bool Attribute::hasValue() const
	{
		return valueSet;
	}

	inline bool Attribute::isValueOptional() const
	{
		return optional;
	}

	inline const Attribute* Descriptor::getAttribute(const std::string& name) const
	{
		return const_cast<Descriptor*>(this)->getAttribute(name);
	}

	inline const Attribute* Descriptor::getAttributeWithValue(const std::string& name) const
	{
		return const_cast<Descriptor*>(this)->getAttribute(name);
	}

	inline const Attribute& Descriptor::getAttribute(size_t index) const
	{
		return const_cast<Descriptor*>(this)->getAttribute(index);
	}

	inline size_t Descriptor::getNbAttributes() const
	{
		return attributes.size();
	}

	inline size_t Descriptor::getSignature() const
	{
		return signature;
	}

	template<typename T>
	void Attribute::setValue(AttributeType valueType,const T& value,bool optional)
	{
		SPK_ASSERT(valueType == type,"Attribute::setValue<T>(AttributeType,const T&,bool) - The value is not of the right type");

		offset = descriptor->buffer.size();
		const char* valueC = reinterpret_cast<const char*>(&value);
		for (size_t i = 0; i < sizeof(T); ++i)
			descriptor->buffer.push_back(valueC[i]);
		valueSet = true;
		this->optional = optional;
	}

	template<typename T>
	void Attribute::setValues(AttributeType valueType,const T* values,size_t nb,bool optional)
	{
		SPK_ASSERT(valueType == type,"Attribute::setValues<T>(AttributeType,const T&,size_t,bool) - The array of values not of the right type");
		SPK_ASSERT(nb > 0,"Attribute::setValues<T>(AttributeType,const T&,size_t,bool) - An array of size 0 cannot be serialized");

		offset = descriptor->buffer.size();
		const char* nbC = reinterpret_cast<char*>(nb);
		const char* valuesC = reinterpret_cast<const char*>(values);
		for (size_t i = 0; i < sizeof(size_t); ++i)
			descriptor->buffer.push_back(nbC[i]);
		for (size_t i = 0; i < sizeof(T) * nb; ++i)
			descriptor->buffer.push_back(valuesC[i]);
		valueSet = true;
		this->optional = optional;
	}

	template<typename T>
	T Attribute::getValue(AttributeType valueType) const
	{
		SPK_ASSERT(valueType == type,"Attribute::getValue<T>(AttributeType) - The desired value is not of the right type");
		SPK_ASSERT(valueSet,"Attribute::getValue<T>(AttributeType) - The value is not set and therefore cannot be read");

		return *reinterpret_cast<T*>(&descriptor->buffer[offset]);
	}

	template<typename T>
	const std::vector<T> Attribute::getValues(AttributeType valueType) const
	{
		SPK_ASSERT(valueType == type,"Attribute::getValue<T>(AttributeType) - The desired array of values is not of the right type");
		SPK_ASSERT(valueSet,"Attribute::getValues(size_t& nb) - The value is not set and therefore cannot be read");

		size_t nb = *reinterpret_cast<size_t*>(&descriptor->buffer[offset]);
		std::vector<T> tmpBuffer;
		for (size_t i = 0; i < nb; ++i)
			tmpBuffer.push_back(*reinterpret_cast<T*>(&descriptor->buffer[offset + i * sizeof(T)]));
		return tmpBuffer;
	}
}

#endif
