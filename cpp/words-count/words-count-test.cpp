#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "words-count.hpp"
#include "array-allocator.hpp"
#include "random.hpp"
#include <vector>

using namespace ::testing;

struct SimdWordsCounter {
    int64_t operator()(std::string s) const {
        return CountWordsSimd(s.data(), s.length());
    }
};

struct NaiveWordsCount {
    int64_t operator()(std::string s) const {
        return CountWordsNaive(s.data(), s.length());
    }
};

template <typename T>
class WordsCountTest : public Test {};
using CounterTypes = Types<SimdWordsCounter, NaiveWordsCount>;
TYPED_TEST_CASE(WordsCountTest, CounterTypes);

TYPED_TEST(WordsCountTest, HelloWorld) {
    EXPECT_EQ(TypeParam()("hello, world!"), 2);
}

TYPED_TEST(WordsCountTest, LongString) {
    EXPECT_EQ(TypeParam()("hello, world and what about   testing?!?! sOmE-More_CompilcatedCases"), 9);
}

TYPED_TEST(WordsCountTest, VeryLongText) {
    EXPECT_EQ(
        TypeParam()("Nagle's algorithm is a means of improving the efficiency of TCP/IP networks by reducing the number of packets that "
                    "need to be sent over the network. It was defined by John Nagle while working for Ford Aerospace. It was published in "
                    "1984 as a Request for Comments (RFC) with title Congestion Control in IP/TCP Internetworks (see RFC 896)"),
        59);
}

TYPED_TEST(WordsCountTest, VeryVeryLongText) {
    EXPECT_EQ(
        TypeParam()(
            "Beethoven was the grandson of Ludwig van Beethoven (1712–1773), a musician from the town of Mechelen in the Duchy of Brabant "
            "in "
            "the Flemish region of what is now Belgium, who at the age of 21 moved to Bonn.[2][3] Ludwig (he adopted the German cognate of "
            "the "
            "Dutch Lodewijk) was employed as a bass singer at the court of the Elector of Cologne, eventually rising to become, in 1761, "
            "Kapellmeister (music director) and thereafter the pre-eminent musician in Bonn. The portrait he commissioned of himself "
            "towards "
            "the end of his life remained displayed in his grandson's rooms as a talisman of his musical heritage.[4] Ludwig had one son, "
            "Johann (1740–1792), who worked as a tenor in the same musical establishment and gave keyboard and violin lessons to "
            "supplement "
            "his income.[2] Johann married Maria Magdalena Keverich in 1767; she was the daughter of Johann Heinrich Keverich (1701–1751), "
            "who "
            "had been the head chef at the court of the Archbishopric of Trier.[5]\n"
            "\n"
            "Beethoven was born of this marriage in Bonn. There is no authentic record of the date of his birth; however, the registry of "
            "his "
            "baptism, in a Catholic service at the Parish of St. Regius on 17 December 1770, survives.[6] As children of that era were "
            "traditionally baptised the day after birth in the Catholic Rhine country, and it is known that Beethoven's family and his "
            "teacher "
            "Johann Albrechtsberger celebrated his birthday on 16 December, most scholars accept 16 December 1770 as his date of "
            "birth.[7][8] "
            "Of the seven children born to Johann van Beethoven, only Ludwig, the second-born, and two younger brothers survived infancy. "
            "Kaspar Anton Karl was born on 8 April 1774, and Nikolaus Johann, the youngest, was born on 2 October 1776.[9]\n"
            "\n"
            "Beethoven's first music teacher was his father. He later had other local teachers: the court organist Gilles van den Eeden "
            "(d. "
            "1782), Tobias Friedrich Pfeiffer (a family friend, who provided keyboard tuition), and Franz Rovantini (a relative, who "
            "instructed him in playing the violin and viola).[2] From the outset his tuition regime, which began in his fifth year, was "
            "harsh "
            "and intensive, often reducing him to tears; with the involvement of the insomniac Pfeiffer there were irregular late-night "
            "sessions with the young Beethoven being dragged from his bed to the keyboard.[10] His musical talent was obvious at a young "
            "age. "
            "Johann, aware of Leopold Mozart's successes in this area (with his son Wolfgang and daughter Nannerl), attempted to promote "
            "his "
            "son as a child prodigy, claiming that Beethoven was six (he was seven) on the posters for his first public performance in "
            "March "
            "1778.[11]\n"
            "\n"
            "Some time after 1779, Beethoven began his studies with his most important teacher in Bonn, Christian Gottlob Neefe, who was "
            "appointed the Court's Organist in that year.[12] Neefe taught him composition, and by March 1783 had helped him write his "
            "first "
            "published composition: a set of keyboard variations (WoO 63).[9] Beethoven soon began working with Neefe as assistant "
            "organist, "
            "at first unpaid (1781), and then as a paid employee (1784) of the court chapel conducted by the Kapellmeister Andrea Luchesi. "
            "His "
            "first three piano sonatas, named \"Kurfürst\" (\"Elector\") for their dedication to the Elector Maximilian Friedrich "
            "(1708–1784), "
            "were published in 1783. Maximilian Frederick noticed his talent early, and subsidised and encouraged the young man's musical "
            "studies"),
        531);
}

TYPED_TEST(WordsCountTest, BruteTest) {
    auto random = Random(0);
    std::string string = "";
    for (size_t i = 0; i < 1000; i++) {
        string += "hello";
        auto spaces = random.NextInt(1, 10);
        for (int s = 0; s < spaces; s++) {
            string.push_back(' ');
        }
    }
    EXPECT_EQ(TypeParam()(string), 1000);
}