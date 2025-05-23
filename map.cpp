#include "lib.h"

unordered_map<string, string> telexMap = {
    {"a", "â"},
    {"d", "đ"},
    {"e", "ê"},
    {"o", "ô"}
};

unordered_map<string, string> telexMapW = {
    {"a", "ă"},
    {"o", "ơ"},
    {"u", "ư"}, 
};

unordered_map<string, unordered_map<char, string>> accentMap = {
    {"a", {{'s', "á"}, {'f', "à"}, {'r', "ả"}, {'x', "ã"}, {'j', "ạ"}}},
    {"ă", {{'s', "ắ"}, {'f', "ằ"}, {'r', "ẳ"}, {'x', "ẵ"}, {'j', "ặ"}}},
    {"â", {{'s', "ấ"}, {'f', "ầ"}, {'r', "ẩ"}, {'x', "ẫ"}, {'j', "ậ"}}},
    {"e", {{'s', "é"}, {'f', "è"}, {'r', "ẻ"}, {'x', "ẽ"}, {'j', "ẹ"}}},
    {"ê", {{'s', "ế"}, {'f', "ề"}, {'r', "ể"}, {'x', "ễ"}, {'j', "ệ"}}},
    {"i", {{'s', "í"}, {'f', "ì"}, {'r', "ỉ"}, {'x', "ĩ"}, {'j', "ị"}}},
    {"o", {{'s', "ó"}, {'f', "ò"}, {'r', "ỏ"}, {'x', "õ"}, {'j', "ọ"}}},
    {"ô", {{'s', "ố"}, {'f', "ồ"}, {'r', "ổ"}, {'x', "ỗ"}, {'j', "ộ"}}},
    {"ơ", {{'s', "ớ"}, {'f', "ờ"}, {'r', "ở"}, {'x', "ỡ"}, {'j', "ợ"}}},
    {"u", {{'s', "ú"}, {'f', "ù"}, {'r', "ủ"}, {'x', "ũ"}, {'j', "ụ"}}},
    {"ư", {{'s', "ứ"}, {'f', "ừ"}, {'r', "ử"}, {'x', "ữ"}, {'j', "ự"}}},
    {"y", {{'s', "ý"}, {'f', "ỳ"}, {'r', "ỷ"}, {'x', "ỹ"}, {'j', "ỵ"}}},
};

const vector<string> priority = {
    "â", "ê", "ô", "ă", "ơ", "ư", "u", "e", "a", "o", "i", "y"
};

unordered_map<string, pair<string, char>> reverseTelexTransform = {
    {"ă", {"a", 'a'}}, {"ắ", {"ấ", 'a'}}, {"ằ", {"ầ", 'a'}}, {"ẳ", {"ẩ", 'a'}}, {"ẵ", {"ẫ", 'a'}}, {"ặ", {"ậ", 'a'}},

    {"â", {"a", 'w'}}, {"ấ", {"ắ", 'w'}}, {"ầ", {"ằ", 'w'}}, {"ẩ", {"ẳ", 'w'}}, {"ẫ", {"ẵ", 'w'}}, {"ậ", {"ặ", 'w'}},

    {"ô", {"o", 'w'}}, {"ố", {"ớ", 'w'}}, {"ồ", {"ờ", 'w'}}, {"ổ", {"ở", 'w'}}, {"ỗ", {"ỡ", 'w'}}, {"ộ", {"ợ", 'w'}},

    {"ơ", {"o", 'o'}}, {"ớ", {"ố", 'o'}}, {"ờ", {"ồ", 'o'}}, {"ở", {"ổ", 'o'}}, {"ỡ", {"ỗ", 'o'}}, {"ợ", {"ộ", 'o'}},
};

unordered_map<string, vector<string>> checkToneExist = {
    {"s", {"á", "ắ", "ấ", "é", "ế", "í", "ó", "ố", "ớ", "ú", "ứ", "ý"}},
    {"f", {"à", "ằ", "ầ", "è", "ề", "ì", "ò", "ồ", "ờ", "ù", "ừ", "ỳ"}},
    {"r", {"ả", "ẳ", "ẩ", "ẻ", "ể", "ỉ", "ỏ", "ổ", "ở", "ủ", "ử", "ỷ"}},
    {"x", {"ã", "ẵ", "ẫ", "ẽ", "ễ", "ĩ", "õ", "ỗ", "ỡ", "ũ", "ữ", "ỹ"}},
    {"j", {"ạ", "ặ", "ậ", "ẹ", "ệ", "ị", "ọ", "ộ", "ợ", "ụ", "ự", "ỵ"}}
};

unordered_map<char, string> checkTelexExist = {
    {'a', "â"},
    {'e', "ê"},
    {'w', "ơ"},
    {'o', "ô"}, 
    {'d', "đ"}
};


// unordered_map<char, unordered_map<char, char>> reverseAccentMap = {
//     {'s', {
//         {'á', 'a'}, {'ắ', 'ă'}, {'ấ', 'â'}, {'é', 'e'}, {'ế', 'ê'},
//         {'í', 'i'}, {'ó', 'o'}, {'ố', 'ô'}, {'ớ', 'ơ'}, {'ú', 'u'},
//         {'ứ', 'ư'}, {'ý', 'y'}
//     }},
//     {'f', {
//         {'à', 'a'}, {'ằ', 'ă'}, {'ầ', 'â'}, {'è', 'e'}, {'ề', 'ê'},
//         {'ì', 'i'}, {'ò', 'o'}, {'ồ', 'ô'}, {'ờ', 'ơ'}, {'ù', 'u'},
//         {'ừ', 'ư'}, {'ỳ', 'y'}
//     }},
//     {'r', {
//         {'ả', 'a'}, {'ẳ', 'ă'}, {'ẩ', 'â'}, {'ẻ', 'e'}, {'ể', 'ê'},
//         {'ỉ', 'i'}, {'ỏ', 'o'}, {'ổ', 'ô'}, {'ở', 'ơ'}, {'ủ', 'u'},
//         {'ử', 'ư'}, {'ỷ', 'y'}
//     }},
//     {'x', {
//         {'ã', 'a'}, {'ẵ', 'ă'}, {'ẫ', 'â'}, {'ẽ', 'e'}, {'ễ', 'ê'},
//         {'ĩ', 'i'}, {'õ', 'o'}, {'ỗ', 'ô'}, {'ỡ', 'ơ'}, {'ũ', 'u'},
//         {'ữ', 'ư'}, {'ỹ', 'y'}
//     }},
//     {'j', {
//         {'ạ', 'a'}, {'ặ', 'ă'}, {'ậ', 'â'}, {'ẹ', 'e'}, {'ệ', 'ê'},
//         {'ị', 'i'}, {'ọ', 'o'}, {'ộ', 'ô'}, {'ợ', 'ơ'}, {'ụ', 'u'},
//         {'ự', 'ư'}, {'ỵ', 'y'}
//     }},
// };

