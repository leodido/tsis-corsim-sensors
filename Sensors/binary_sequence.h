/*******************************************************************************
    Copyright 2013 Leonardo Di Donato

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    Application: tsis-corsim-sensors

    Description:
    Helper class for binary sequences.
*******************************************************************************/

#ifndef CBINARYSEQUENCE
#define CBINARYSEQUENCE

#pragma once

class CInteger;

class CBinarySequence : public CObject
{
public:
    CBinarySequence();
    virtual ~CBinarySequence();

    static CBinarySequence *convert(int value);

    CTypedPtrList<CPtrList, CInteger *> sequence;
};

#endif // CBINARYSEQUENCE
