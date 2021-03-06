#ifndef MPED_DELIMITEDDSEQUENCE_H
#define MPED_DELIMITEDDSEQUENCE_H

#include <map>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include "AbstractSequence.h"

class DelimitedSequence : public AbstractSequence {
private:

    std::map<std::string, unsigned> mapping;

    std::vector<std::string> sequence;
    std::vector<std::string> sigma;

    // TODO: to provide tokenization for different delimitators
    void tokenize() {
        std::istringstream ss{this->base};
        using StrIt = std::istream_iterator<std::string>;
        std::vector<std::string> container{StrIt{ss}, StrIt{}};

        sequence = container;
    }

    void create_sigma() {
        std::vector<std::string> container(sequence);
        std::sort(container.begin(), container.end());
        auto it = std::unique(container.begin(), container.end());
        for (auto i = container.begin(); i != it; i++)
            sigma.push_back(*i);
    }


    void extract_tokens() {
        // (1) tokenize seq for whitespace and (1.1) save the tokenized base
        tokenize();

        // (2) save distinct tokens
        create_sigma();
    }

    void define_mapping() {
        unsigned i = 0;
        for (auto x: sigma)
            mapping.insert(std::pair<std::string, unsigned>(x, i++));
    }

    void fill_representation() {
        // unsigned representation of the base
        for (const auto& x: sequence)
            sequence_repr.push_back(mapping.at(x));

        // unsigned representation of the token alphabet
        for (const auto& x: sigma)
            sigma_repr.push_back(mapping.at(x));
    }

    void init() {
        extract_tokens();
        define_mapping();
        fill_representation();
    }

public:
    DelimitedSequence(const std::string& s) : AbstractSequence(s) { init(); }
    DelimitedSequence(const char* c) : AbstractSequence(c) { init(); }

    const std::vector<std::string> &getSequence() const { return sequence; }
    const std::vector<std::string> &getSigma() const { return sigma; }
    const std::map<std::string, unsigned int> &getMapping() const { return mapping; }
};

#endif //MPED_DELIMITEDDSEQUENCE_H
