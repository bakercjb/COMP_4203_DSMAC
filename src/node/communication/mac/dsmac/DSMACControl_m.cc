//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/dsmac/DSMACControl.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "DSMACControl_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("DSMACCommandDef");
    if (!e) enums.getInstance()->add(e = new cEnum("DSMACCommandDef"));
    e->insert(SET_DUTY_CYCLE, "SET_DUTY_CYCLE");
    e->insert(SET_LISTEN_INTERVAL, "SET_LISTEN_INTERVAL");
    e->insert(SET_BEACON_INTERVAL_FRACTION, "SET_BEACON_INTERVAL_FRACTION");
    e->insert(SET_PROB_TX, "SET_PROB_TX");
    e->insert(SET_NUM_TX, "SET_NUM_TX");
    e->insert(SET_RANDOM_TX_OFFSET, "SET_RANDOM_TX_OFFSET");
    e->insert(SET_RETX_INTERVAL, "SET_RETX_INTERVAL");
    e->insert(SET_BACKOFF_TYPE, "SET_BACKOFF_TYPE");
    e->insert(SET_BACKOFF_BASE_VALUE, "SET_BACKOFF_BASE_VALUE");
);

Register_Class(DSMACControlCommand);

DSMACControlCommand::DSMACControlCommand(const char *name, int kind) : ::cMessage(name,kind)
{
    this->DSMACCommandKind_var = 0;
    this->parameter_var = 0;
}

DSMACControlCommand::DSMACControlCommand(const DSMACControlCommand& other) : ::cMessage(other)
{
    copy(other);
}

DSMACControlCommand::~DSMACControlCommand()
{
}

DSMACControlCommand& DSMACControlCommand::operator=(const DSMACControlCommand& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void DSMACControlCommand::copy(const DSMACControlCommand& other)
{
    this->DSMACCommandKind_var = other.DSMACCommandKind_var;
    this->parameter_var = other.parameter_var;
}

void DSMACControlCommand::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->DSMACCommandKind_var);
    doPacking(b,this->parameter_var);
}

void DSMACControlCommand::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->DSMACCommandKind_var);
    doUnpacking(b,this->parameter_var);
}

int DSMACControlCommand::getDSMACCommandKind() const
{
    return DSMACCommandKind_var;
}

void DSMACControlCommand::setDSMACCommandKind(int DSMACCommandKind)
{
    this->DSMACCommandKind_var = DSMACCommandKind;
}

double DSMACControlCommand::getParameter() const
{
    return parameter_var;
}

void DSMACControlCommand::setParameter(double parameter)
{
    this->parameter_var = parameter;
}

class DSMACControlCommandDescriptor : public cClassDescriptor
{
  public:
    DSMACControlCommandDescriptor();
    virtual ~DSMACControlCommandDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DSMACControlCommandDescriptor);

DSMACControlCommandDescriptor::DSMACControlCommandDescriptor() : cClassDescriptor("DSMACControlCommand", "cMessage")
{
}

DSMACControlCommandDescriptor::~DSMACControlCommandDescriptor()
{
}

bool DSMACControlCommandDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DSMACControlCommand *>(obj)!=NULL;
}

const char *DSMACControlCommandDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DSMACControlCommandDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int DSMACControlCommandDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *DSMACControlCommandDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "DSMACCommandKind",
        "parameter",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int DSMACControlCommandDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DSMACCommandKind")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "parameter")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DSMACControlCommandDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *DSMACControlCommandDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSMACCommandDef";
            return NULL;
        default: return NULL;
    }
}

int DSMACControlCommandDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DSMACControlCommand *pp = (DSMACControlCommand *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DSMACControlCommandDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACControlCommand *pp = (DSMACControlCommand *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDSMACCommandKind());
        case 1: return double2string(pp->getParameter());
        default: return "";
    }
}

bool DSMACControlCommandDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DSMACControlCommand *pp = (DSMACControlCommand *)object; (void)pp;
    switch (field) {
        case 0: pp->setDSMACCommandKind(string2long(value)); return true;
        case 1: pp->setParameter(string2double(value)); return true;
        default: return false;
    }
}

const char *DSMACControlCommandDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *DSMACControlCommandDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACControlCommand *pp = (DSMACControlCommand *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


