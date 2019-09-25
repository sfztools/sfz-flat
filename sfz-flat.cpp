// Copyright (c) 2019, Paul Ferrand
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Parser.h"
#include "StringViewHelpers.h"
#include <iostream>
#include <string_view>
#include <absl/flags/parse.h>
#include <absl/types/span.h>

class PrintingParser: public sfz::Parser
{
public:
    int getNumRegions() const noexcept { return numRegions; }
    int getNumGroups() const noexcept { return numGroups; }
    int getNumMasters() const noexcept { return numMasters; }
    int getNumCurves() const noexcept { return numCurves; }
protected:
    void callback(absl::string_view header, const std::vector<sfz::Opcode>& members) final
    {
        switch (hash(header))
        {
        case hash("global"): 
            globalMembers = members;
            masterMembers.clear();
            groupMembers.clear();
            break;
        case hash("master"): 
            masterMembers = members;
            groupMembers.clear();
            break;
        case hash("group"):
            groupMembers = members;
            break;
        case hash("region"): 
            std::cout << "<" << header << ">" << ' ';
            printMembers(globalMembers);
            printMembers(masterMembers);
            printMembers(groupMembers);
            printMembers(members);
            std::cout << '\n';
            break;
        default:
            std::cout << "<" << header << ">" << ' ';
            printMembers(members);
            std::cout << '\n';
            break;
        }
    }
private:
    int numRegions { 0 };
    int numGroups { 0 };
    int numMasters { 0 };
    int numCurves { 0 };
    std::vector<sfz::Opcode> globalMembers;
    std::vector<sfz::Opcode> masterMembers;
    std::vector<sfz::Opcode> groupMembers;
    void printMembers(const std::vector<sfz::Opcode>& members)
    {
        for (auto& member: members)
        {
            std::cout << member.opcode;
            if (member.parameter) 
                std::cout << +*member.parameter;
            std::cout << "=" << member.value;
            std::cout << ' ';
        }
    }
};

int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);
    auto arguments = absl::ParseCommandLine(argc, argv);
    auto filesToParse = absl::MakeConstSpan(arguments).subspan(1);    
    PrintingParser parser;
    parser.loadSfzFile(filesToParse[0]);
    return 0;
}
