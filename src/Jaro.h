#ifndef MPED_JARO_H
#define MPED_JARO_H

#include <iostream>
#include <vector>
#include "DelimitedSequence.h"
#include "MatchingSchema.h"
#include "Utility.h"

class Jaro {
public:
    static double compute(const DelimitedSequence& a, const DelimitedSequence& b, const MatchingSchema& ms) {
        auto* sig1_index = new unsigned[a.sigma_len()];
        for (unsigned i = 0; i < a.sigma_len(); ++i)
            sig1_index[a.getSigma_repr()[i]] = i;

        auto* sig2_index = new unsigned[b.sigma_len()];
        for (unsigned i = 0; i < b.sigma_len(); ++i)
            sig2_index[b.getSigma_repr()[i]] = i;

        const uint l1 = a.seq_len(), l2 = b.seq_len();

        if (l1 == 0)
            return l2 == 0 ? 1.0 : 0.0;

        const uint match_distance = std::max(l1, l2) / 2 - 1;
        bool s1_matches[l1];
        bool s2_matches[l2];
        std::fill(s1_matches, s1_matches + l1, false);
        std::fill(s2_matches, s2_matches + l2, false);
        uint matches = 0;

        for (uint i = 0; i < l1; i++) {
            const int endd = std::min(i + match_distance + 1, l2);
            for (int k = max(0, i - match_distance); k < endd; k++) {
                if (!s2_matches[k] && !ms.ms[sig1_index[a.getSeq_repr()[i]]][sig2_index[b.getSeq_repr()[k]]]) {
                    s1_matches[i] = true;
                    s2_matches[k] = true;
                    matches++;
                    break;
                }
            }
        }



        if (matches == 0)
            return 0.0;

        double t = 0.0;
        uint k = 0;
        for (uint i = 0; i < l1; i++)
            if (s1_matches[i]) {
                while (!s2_matches[k]) k++;
                if (ms.ms[sig1_index[a.getSeq_repr()[i]]][sig2_index[b.getSeq_repr()[k]]]) t += 0.5;
                k++;
            }

        const double m = matches;
        return (m / l1 + m / l2 + (m - t) / m) / 3.0;
    }

    static double jaro(const std::string s1, const std::string s2) {
        const uint l1 = s1.length(), l2 = s2.length();
        if (l1 == 0)
            return l2 == 0 ? 1.0 : 0.0;
        const uint match_distance = std::max(l1, l2) / 2 - 1;
        bool s1_matches[l1];
        bool s2_matches[l2];
        std::fill(s1_matches, s1_matches + l1, false);
        std::fill(s2_matches, s2_matches + l2, false);
        uint matches = 0;
        for (uint i = 0; i < l1; i++)
        {
            const int end = std::min(i + match_distance + 1, l2);
            for (int k = std::max(0u, i - match_distance); k < end; k++)
                if (!s2_matches[k] && s1[i] == s2[k])
                {
                    s1_matches[i] = true;
                    s2_matches[k] = true;
                    matches++;
                    break;
                }
        }
        if (matches == 0)
            return 0.0;
        double t = 0.0;
        uint k = 0;
        for (uint i = 0; i < l1; i++)
            if (s1_matches[i])
            {
                while (!s2_matches[k]) k++;
                if (s1[i] != s2[k]) t += 0.5;
                k++;
            }

        const double m = matches;
        return (m / l1 + m / l2 + (m - t) / m) / 3.0;
    }
};


#endif //MPED_JACCARD_H