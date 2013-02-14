#include "stdafx.h"

#include <math.h>

#include "binary_sequence.h"
#include "integer.h"

CBinarySequence::CBinarySequence() : CObject()
                                   , sequence()
{
}

CBinarySequence::~CBinarySequence()
{
    CInteger* pi = NULL;
    POSITION pos = sequence.GetHeadPosition();
    while (pos != NULL)
    {
        pi = sequence.GetNext(pos);
        delete pi;
    }
    sequence.RemoveAll();
}

CBinarySequence* CBinarySequence::convert(int value)
{
    // this function converts an integer into a binary sequence
    CBinarySequence* sequence = NULL;
    CInteger* pi = NULL;
    int remainder = 0;
    int power = 0;
    // create a binary sequence
    sequence = new CBinarySequence();
    // assume the binary number is less than 2^10
    remainder = value;
    for (power = 9; power > -1; power--)
    {
        pi = new CInteger;
	    double r2 = 2.0;
        if (remainder >= (int) pow(r2, power))
            pi->data = 1;
        else
            pi->data = 0;
        sequence->sequence.AddTail(pi);
        remainder = remainder - (pi->data) * (int) pow(r2, power);
    }

    // the sequence will be reversed
    return sequence;
}