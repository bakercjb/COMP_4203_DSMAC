//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/dsmac/DSMACPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "DSMACPacket_m.h"

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
    cEnum *e = cEnum::find("DSMACPacket_type");
    if (!e) enums.getInstance()->add(e = new cEnum("DSMACPacket_type"));
    e->insert(MAC_DSMAC_BEACON_PACKET, "MAC_DSMAC_BEACON_PACKET");
    e->insert(MAC_DSMAC_ASSOCIATE_PACKET, "MAC_DSMAC_ASSOCIATE_PACKET");
    e->insert(MAC_DSMAC_DATA_PACKET, "MAC_DSMAC_DATA_PACKET");
    e->insert(MAC_DSMAC_ACK_PACKET, "MAC_DSMAC_ACK_PACKET");
    e->insert(MAC_DSMAC_GTS_REQUEST_PACKET, "MAC_DSMAC_GTS_REQUEST_PACKET");
);

DSMACGTSspec::DSMACGTSspec()
{
    owner = 0;
    start = 0;
    length = 0;
}

void doPacking(cCommBuffer *b, DSMACGTSspec& a)
{
    doPacking(b,a.owner);
    doPacking(b,a.start);
    doPacking(b,a.length);
}

void doUnpacking(cCommBuffer *b, DSMACGTSspec& a)
{
    doUnpacking(b,a.owner);
    doUnpacking(b,a.start);
    doUnpacking(b,a.length);
}

class DSMACGTSspecDescriptor : public cClassDescriptor
{
  public:
    DSMACGTSspecDescriptor();
    virtual ~DSMACGTSspecDescriptor();

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

Register_ClassDescriptor(DSMACGTSspecDescriptor);

DSMACGTSspecDescriptor::DSMACGTSspecDescriptor() : cClassDescriptor("DSMACGTSspec", "")
{
}

DSMACGTSspecDescriptor::~DSMACGTSspecDescriptor()
{
}

bool DSMACGTSspecDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DSMACGTSspec *>(obj)!=NULL;
}

const char *DSMACGTSspecDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DSMACGTSspecDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int DSMACGTSspecDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *DSMACGTSspecDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "owner",
        "start",
        "length",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int DSMACGTSspecDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "owner")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "start")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DSMACGTSspecDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *DSMACGTSspecDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DSMACGTSspecDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DSMACGTSspec *pp = (DSMACGTSspec *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DSMACGTSspecDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACGTSspec *pp = (DSMACGTSspec *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->owner);
        case 1: return long2string(pp->start);
        case 2: return long2string(pp->length);
        default: return "";
    }
}

bool DSMACGTSspecDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DSMACGTSspec *pp = (DSMACGTSspec *)object; (void)pp;
    switch (field) {
        case 0: pp->owner = string2long(value); return true;
        case 1: pp->start = string2long(value); return true;
        case 2: pp->length = string2long(value); return true;
        default: return false;
    }
}

const char *DSMACGTSspecDescriptor::getFieldStructName(void *object, int field) const
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

void *DSMACGTSspecDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACGTSspec *pp = (DSMACGTSspec *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DSMACPacket);

DSMACPacket::DSMACPacket(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->DSMACPacketType_var = 0;
    this->leaderNodeid_var = 0;
    this->srcID_var = 0;
    this->dstID_var = 0;
    this->seqNum_var = 0;
    this->sessionIDPkt_var = 0;
    this->beaconOrder_var = 0;
    this->frameOrder_var = 0;
    this->BSN_var = 0;
    this->CAPlength_var = 0;
    this->GTSlength_var = 0;
    GTSlist_arraysize = 0;
    this->GTSlist_var = 0;
}

DSMACPacket::DSMACPacket(const DSMACPacket& other) : ::MacPacket(other)
{
    GTSlist_arraysize = 0;
    this->GTSlist_var = 0;
    copy(other);
}

DSMACPacket::~DSMACPacket()
{
    delete [] GTSlist_var;
}

DSMACPacket& DSMACPacket::operator=(const DSMACPacket& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void DSMACPacket::copy(const DSMACPacket& other)
{
    this->DSMACPacketType_var = other.DSMACPacketType_var;
    this->leaderNodeid_var = other.leaderNodeid_var;
    this->srcID_var = other.srcID_var;
    this->dstID_var = other.dstID_var;
    this->seqNum_var = other.seqNum_var;
    this->sessionIDPkt_var = other.sessionIDPkt_var;
    this->beaconOrder_var = other.beaconOrder_var;
    this->frameOrder_var = other.frameOrder_var;
    this->BSN_var = other.BSN_var;
    this->CAPlength_var = other.CAPlength_var;
    this->GTSlength_var = other.GTSlength_var;
    delete [] this->GTSlist_var;
    this->GTSlist_var = (other.GTSlist_arraysize==0) ? NULL : new DSMACGTSspec[other.GTSlist_arraysize];
    GTSlist_arraysize = other.GTSlist_arraysize;
    for (unsigned int i=0; i<GTSlist_arraysize; i++)
        this->GTSlist_var[i] = other.GTSlist_var[i];
}

void DSMACPacket::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->DSMACPacketType_var);
    doPacking(b,this->leaderNodeid_var);
    doPacking(b,this->srcID_var);
    doPacking(b,this->dstID_var);
    doPacking(b,this->seqNum_var);
    doPacking(b,this->sessionIDPkt_var);
    doPacking(b,this->beaconOrder_var);
    doPacking(b,this->frameOrder_var);
    doPacking(b,this->BSN_var);
    doPacking(b,this->CAPlength_var);
    doPacking(b,this->GTSlength_var);
    b->pack(GTSlist_arraysize);
    doPacking(b,this->GTSlist_var,GTSlist_arraysize);
}

void DSMACPacket::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->DSMACPacketType_var);
    doUnpacking(b,this->leaderNodeid_var);
    doUnpacking(b,this->srcID_var);
    doUnpacking(b,this->dstID_var);
    doUnpacking(b,this->seqNum_var);
    doUnpacking(b,this->sessionIDPkt_var);
    doUnpacking(b,this->beaconOrder_var);
    doUnpacking(b,this->frameOrder_var);
    doUnpacking(b,this->BSN_var);
    doUnpacking(b,this->CAPlength_var);
    doUnpacking(b,this->GTSlength_var);
    delete [] this->GTSlist_var;
    b->unpack(GTSlist_arraysize);
    if (GTSlist_arraysize==0) {
        this->GTSlist_var = 0;
    } else {
        this->GTSlist_var = new DSMACGTSspec[GTSlist_arraysize];
        doUnpacking(b,this->GTSlist_var,GTSlist_arraysize);
    }
}

int DSMACPacket::getDSMACPacketType() const
{
    return DSMACPacketType_var;
}

void DSMACPacket::setDSMACPacketType(int DSMACPacketType)
{
    this->DSMACPacketType_var = DSMACPacketType;
}

int DSMACPacket::getLeaderNodeid() const
{
    return leaderNodeid_var;
}

void DSMACPacket::setLeaderNodeid(int leaderNodeid)
{
    this->leaderNodeid_var = leaderNodeid;
}

int DSMACPacket::getSrcID() const
{
    return srcID_var;
}

void DSMACPacket::setSrcID(int srcID)
{
    this->srcID_var = srcID;
}

int DSMACPacket::getDstID() const
{
    return dstID_var;
}

void DSMACPacket::setDstID(int dstID)
{
    this->dstID_var = dstID;
}

int DSMACPacket::getSeqNum() const
{
    return seqNum_var;
}

void DSMACPacket::setSeqNum(int seqNum)
{
    this->seqNum_var = seqNum;
}

int DSMACPacket::getSessionIDPkt() const
{
    return sessionIDPkt_var;
}

void DSMACPacket::setSessionIDPkt(int sessionIDPkt)
{
    this->sessionIDPkt_var = sessionIDPkt;
}

int DSMACPacket::getBeaconOrder() const
{
    return beaconOrder_var;
}

void DSMACPacket::setBeaconOrder(int beaconOrder)
{
    this->beaconOrder_var = beaconOrder;
}

int DSMACPacket::getFrameOrder() const
{
    return frameOrder_var;
}

void DSMACPacket::setFrameOrder(int frameOrder)
{
    this->frameOrder_var = frameOrder;
}

int DSMACPacket::getBSN() const
{
    return BSN_var;
}

void DSMACPacket::setBSN(int BSN)
{
    this->BSN_var = BSN;
}

int DSMACPacket::getCAPlength() const
{
    return CAPlength_var;
}

void DSMACPacket::setCAPlength(int CAPlength)
{
    this->CAPlength_var = CAPlength;
}

int DSMACPacket::getGTSlength() const
{
    return GTSlength_var;
}

void DSMACPacket::setGTSlength(int GTSlength)
{
    this->GTSlength_var = GTSlength;
}

void DSMACPacket::setGTSlistArraySize(unsigned int size)
{
    DSMACGTSspec *GTSlist_var2 = (size==0) ? NULL : new DSMACGTSspec[size];
    unsigned int sz = GTSlist_arraysize < size ? GTSlist_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        GTSlist_var2[i] = this->GTSlist_var[i];
    GTSlist_arraysize = size;
    delete [] this->GTSlist_var;
    this->GTSlist_var = GTSlist_var2;
}

unsigned int DSMACPacket::getGTSlistArraySize() const
{
    return GTSlist_arraysize;
}

DSMACGTSspec& DSMACPacket::getGTSlist(unsigned int k)
{
    if (k>=GTSlist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", GTSlist_arraysize, k);
    return GTSlist_var[k];
}

void DSMACPacket::setGTSlist(unsigned int k, const DSMACGTSspec& GTSlist)
{
    if (k>=GTSlist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", GTSlist_arraysize, k);
    this->GTSlist_var[k] = GTSlist;
}

class DSMACPacketDescriptor : public cClassDescriptor
{
  public:
    DSMACPacketDescriptor();
    virtual ~DSMACPacketDescriptor();

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

Register_ClassDescriptor(DSMACPacketDescriptor);

DSMACPacketDescriptor::DSMACPacketDescriptor() : cClassDescriptor("DSMACPacket", "MacPacket")
{
}

DSMACPacketDescriptor::~DSMACPacketDescriptor()
{
}

bool DSMACPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DSMACPacket *>(obj)!=NULL;
}

const char *DSMACPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DSMACPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount(object) : 12;
}

unsigned int DSMACPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *DSMACPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "DSMACPacketType",
        "leaderNodeid",
        "srcID",
        "dstID",
        "seqNum",
        "sessionIDPkt",
        "beaconOrder",
        "frameOrder",
        "BSN",
        "CAPlength",
        "GTSlength",
        "GTSlist",
    };
    return (field>=0 && field<12) ? fieldNames[field] : NULL;
}

int DSMACPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DSMACPacketType")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "leaderNodeid")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcID")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstID")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNum")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "sessionIDPkt")==0) return base+5;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconOrder")==0) return base+6;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameOrder")==0) return base+7;
    if (fieldName[0]=='B' && strcmp(fieldName, "BSN")==0) return base+8;
    if (fieldName[0]=='C' && strcmp(fieldName, "CAPlength")==0) return base+9;
    if (fieldName[0]=='G' && strcmp(fieldName, "GTSlength")==0) return base+10;
    if (fieldName[0]=='G' && strcmp(fieldName, "GTSlist")==0) return base+11;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DSMACPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "DSMACGTSspec",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : NULL;
}

const char *DSMACPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSMACPacket_type";
            return NULL;
        default: return NULL;
    }
}

int DSMACPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DSMACPacket *pp = (DSMACPacket *)object; (void)pp;
    switch (field) {
        case 11: return pp->getGTSlistArraySize();
        default: return 0;
    }
}

std::string DSMACPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACPacket *pp = (DSMACPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDSMACPacketType());
        case 1: return long2string(pp->getLeaderNodeid());
        case 2: return long2string(pp->getSrcID());
        case 3: return long2string(pp->getDstID());
        case 4: return long2string(pp->getSeqNum());
        case 5: return long2string(pp->getSessionIDPkt());
        case 6: return long2string(pp->getBeaconOrder());
        case 7: return long2string(pp->getFrameOrder());
        case 8: return long2string(pp->getBSN());
        case 9: return long2string(pp->getCAPlength());
        case 10: return long2string(pp->getGTSlength());
        case 11: {std::stringstream out; out << pp->getGTSlist(i); return out.str();}
        default: return "";
    }
}

bool DSMACPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DSMACPacket *pp = (DSMACPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDSMACPacketType(string2long(value)); return true;
        case 1: pp->setLeaderNodeid(string2long(value)); return true;
        case 2: pp->setSrcID(string2long(value)); return true;
        case 3: pp->setDstID(string2long(value)); return true;
        case 4: pp->setSeqNum(string2long(value)); return true;
        case 5: pp->setSessionIDPkt(string2long(value)); return true;
        case 6: pp->setBeaconOrder(string2long(value)); return true;
        case 7: pp->setFrameOrder(string2long(value)); return true;
        case 8: pp->setBSN(string2long(value)); return true;
        case 9: pp->setCAPlength(string2long(value)); return true;
        case 10: pp->setGTSlength(string2long(value)); return true;
        default: return false;
    }
}

const char *DSMACPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 11: return opp_typename(typeid(DSMACGTSspec));
        default: return NULL;
    };
}

void *DSMACPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DSMACPacket *pp = (DSMACPacket *)object; (void)pp;
    switch (field) {
        case 11: return (void *)(&pp->getGTSlist(i)); break;
        default: return NULL;
    }
}


