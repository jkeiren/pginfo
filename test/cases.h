#ifndef _CASES_H
#define _CASES_H

#include <string>

const std::string
BUFFER_NODEADLOCK(
  "parity 3;\n"
  "0 0 1 1;\n"
  "1 0 1 2, 3;\n"
  "2 0 1 1;\n"
  "3 0 1 1;\n"
);

const std::string
ABP_NODEADLOCK(
  "parity 74;\n"
  "0 0 1 1;\n"
  "1 0 1 2, 3;\n"
  "2 0 1 4;\n"
  "3 0 1 5;\n"
  "4 0 1 6, 7;\n"
  "5 0 1 8, 9;\n"
  "6 0 1 10;\n"
  "7 0 1 11;\n"
  "8 0 1 12;\n"
  "9 0 1 13;\n"
  "10 0 1 14;\n"
  "11 0 1 15;\n"
  "12 0 1 16;\n"
  "13 0 1 17;\n"
  "14 0 1 18;\n"
  "15 0 1 19, 20;\n"
  "16 0 1 21;\n"
  "17 0 1 22, 23;\n"
  "18 0 1 24, 25;\n"
  "19 0 1 2;\n"
  "20 0 1 2;\n"
  "21 0 1 26, 27;\n"
  "22 0 1 3;\n"
  "23 0 1 3;\n"
  "24 0 1 28;\n"
  "25 0 1 29;\n"
  "26 0 1 28;\n"
  "27 0 1 30;\n"
  "28 0 1 31, 32;\n"
  "29 0 1 33;\n"
  "30 0 1 34;\n"
  "31 0 1 35;\n"
  "32 0 1 36;\n"
  "33 0 1 37, 38;\n"
  "34 0 1 39, 40;\n"
  "35 0 1 41, 42;\n"
  "36 0 1 43, 44;\n"
  "37 0 1 45;\n"
  "38 0 1 45;\n"
  "39 0 1 46;\n"
  "40 0 1 46;\n"
  "41 0 1 47;\n"
  "42 0 1 48;\n"
  "43 0 1 49;\n"
  "44 0 1 50;\n"
  "45 0 1 18;\n"
  "46 0 1 21;\n"
  "47 0 1 51;\n"
  "48 0 1 52;\n"
  "49 0 1 53;\n"
  "50 0 1 54;\n"
  "51 0 1 55;\n"
  "52 0 1 56, 57;\n"
  "53 0 1 58;\n"
  "54 0 1 59, 60;\n"
  "55 0 1 61, 62;\n"
  "56 0 1 31;\n"
  "57 0 1 31;\n"
  "58 0 1 63, 64;\n"
  "59 0 1 32;\n"
  "60 0 1 32;\n"
  "61 0 1 1;\n"
  "62 0 1 65;\n"
  "63 0 1 1;\n"
  "64 0 1 66;\n"
  "65 0 1 67;\n"
  "66 0 1 68;\n"
  "67 0 1 69, 70;\n"
  "68 0 1 71, 72;\n"
  "69 0 1 73;\n"
  "70 0 1 73;\n"
  "71 0 1 74;\n"
  "72 0 1 74;\n"
  "73 0 1 55;\n"
  "74 0 1 58;\n"
);

const std::string
ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR(
  "parity 131;\n"
  "0 2 1 1;\n"
  "1 2 1 2, 3, 4, 5;\n"
  "2 2 1 6;\n"
  "3 2 1 7;\n"
  "4 2 1 95;\n"
  "5 2 1 96;\n"
  "6 2 1 8, 9;\n"
  "7 2 1 10, 11;\n"
  "8 2 1 12;\n"
  "9 2 1 13;\n"
  "10 2 1 14;\n"
  "11 2 1 15;\n"
  "12 2 1 20;\n"
  "13 2 1 21;\n"
  "14 2 1 22;\n"
  "15 2 1 23;\n"
  "16 2 1 99;\n"
  "17 2 1 100;\n"
  "18 2 1 101;\n"
  "19 2 1 102;\n"
  "20 2 1 24;\n"
  "21 2 1 25, 26;\n"
  "22 2 1 27;\n"
  "23 2 1 28, 29;\n"
  "24 2 1 30, 31;\n"
  "25 2 1 2;\n"
  "26 2 1 2;\n"
  "27 2 1 32, 33;\n"
  "28 2 1 3;\n"
  "29 2 1 3;\n"
  "30 2 1 34;\n"
  "31 2 1 35;\n"
  "32 2 1 34;\n"
  "33 2 1 36;\n"
  "34 2 1 41, 42, 43, 44;\n"
  "35 2 1 45;\n"
  "36 2 1 46;\n"
  "37 2 1 109;\n"
  "38 2 1 110;\n"
  "39 2 1 111;\n"
  "40 2 1 112;\n"
  "41 2 1 47;\n"
  "42 2 1 48;\n"
  "43 2 1 113;\n"
  "44 2 1 114;\n"
  "45 2 1 49, 50;\n"
  "46 2 1 51, 52;\n"
  "47 2 1 53, 54;\n"
  "48 2 1 55, 56;\n"
  "49 2 1 57;\n"
  "50 2 1 57;\n"
  "51 2 1 58;\n"
  "52 2 1 58;\n"
  "53 2 1 59;\n"
  "54 2 1 60;\n"
  "55 2 1 61;\n"
  "56 2 1 62;\n"
  "57 2 1 24;\n"
  "58 2 1 27;\n"
  "59 2 1 67;\n"
  "60 2 1 68;\n"
  "61 2 1 69;\n"
  "62 2 1 70;\n"
  "63 2 1 117;\n"
  "64 2 1 118;\n"
  "65 2 1 119;\n"
  "66 2 1 120;\n"
  "67 2 1 71;\n"
  "68 2 1 72, 73;\n"
  "69 2 1 74;\n"
  "70 2 1 75, 76;\n"
  "71 2 1 77, 78;\n"
  "72 2 1 41;\n"
  "73 2 1 41;\n"
  "74 2 1 79, 80;\n"
  "75 2 1 42;\n"
  "76 2 1 42;\n"
  "77 2 1 1;\n"
  "78 2 1 81;\n"
  "79 2 1 1;\n"
  "80 2 1 82;\n"
  "81 2 1 87;\n"
  "82 2 1 88;\n"
  "83 2 1 127;\n"
  "84 2 1 128;\n"
  "85 2 1 129;\n"
  "86 2 1 130;\n"
  "87 2 1 89, 90;\n"
  "88 2 1 91, 92;\n"
  "89 2 1 93;\n"
  "90 2 1 93;\n"
  "91 2 1 94;\n"
  "92 2 1 94;\n"
  "93 2 1 71;\n"
  "94 2 1 74;\n"
  "95 1 1 97;\n"
  "96 1 1 98;\n"
  "97 1 1 16, 17;\n"
  "98 1 1 18, 19;\n"
  "99 1 1 103;\n"
  "100 1 1 104;\n"
  "101 1 1 105;\n"
  "102 1 1 106;\n"
  "103 1 1 131;\n"
  "104 1 1 107;\n"
  "105 1 1 131;\n"
  "106 1 1 108;\n"
  "107 1 1 37, 38;\n"
  "108 1 1 39, 40;\n"
  "109 1 1 95;\n"
  "110 1 1 95;\n"
  "111 1 1 96;\n"
  "112 1 1 96;\n"
  "113 1 1 115;\n"
  "114 1 1 116;\n"
  "115 1 1 63, 64;\n"
  "116 1 1 65, 66;\n"
  "117 1 1 121;\n"
  "118 1 1 122;\n"
  "119 1 1 123;\n"
  "120 1 1 124;\n"
  "121 1 1 131;\n"
  "122 1 1 125;\n"
  "123 1 1 131;\n"
  "124 1 1 126;\n"
  "125 1 1 83, 84;\n"
  "126 1 1 85, 86;\n"
  "127 1 1 113;\n"
  "128 1 1 113;\n"
  "129 1 1 114;\n"
  "130 1 1 114;\n"
  "131 0 0 131;\n"
);

#endif // _CASES_H
