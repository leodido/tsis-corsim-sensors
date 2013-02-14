#ifndef CBINARYSEQUENCE
#define CBINARYSEQUENCE

class CInteger;

class CBinarySequence : public CObject
{
   public:
      CBinarySequence();
      virtual ~CBinarySequence();

      static CBinarySequence* convert(int value);

      CTypedPtrList<CPtrList, CInteger*> sequence;
};

#endif // CBINARYSEQUENCE
