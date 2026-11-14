#include "spellcorreter/EnglishData.hpp"

namespace sc::en {

const std::vector<Typo>& typos() {
    static const std::vector<Typo> v = {
        {"teh","the",""}, {"hte","the",""}, {"adn","and",""}, {"yuo","you",""}, {"tehy","they",""},
        {"recieve","receive",""}, {"definately","definitely",""}, {"definatly","definitely",""},
        {"seperate","separate",""}, {"occured","occurred",""}, {"untill","until",""},
        {"wich","which",""}, {"becuase","because",""}, {"becasue","because",""},
        {"beleive","believe",""}, {"freind","friend",""}, {"wierd","weird",""},
        {"thier","their",""}, {"alot","a lot","'a lot'는 띄어 씁니다"},
        {"accomodate","accommodate",""}, {"adress","address",""}, {"agressive","aggressive",""},
        {"apparant","apparent",""}, {"arguement","argument",""}, {"begining","beginning",""},
        {"calender","calendar",""}, {"collegue","colleague",""}, {"comming","coming",""},
        {"concious","conscious",""}, {"embarass","embarrass",""}, {"enviroment","environment",""},
        {"existance","existence",""}, {"goverment","government",""}, {"grammer","grammar",""},
        {"immediatly","immediately",""}, {"independant","independent",""},
        {"knowlege","knowledge",""}, {"maintainance","maintenance",""},
        {"millenium","millennium",""}, {"neccessary","necessary",""}, {"necesary","necessary",""},
        {"noticable","noticeable",""}, {"occassion","occasion",""}, {"persistant","persistent",""},
        {"posession","possession",""}, {"prefered","preferred",""}, {"publically","publicly",""},
        {"realy","really",""}, {"refered","referred",""}, {"relevent","relevant",""},
        {"restaraunt","restaurant",""}, {"rythm","rhythm",""}, {"succesful","successful",""},
        {"suprise","surprise",""}, {"tommorow","tomorrow",""}, {"tomorow","tomorrow",""},
        {"truely","truly",""}, {"writting","writing",""}, {"accross","across",""},
        {"irregardless","regardless","'regardless'가 올바른 표현입니다"},
        {"shoudl","should",""}, {"wouldnt","wouldn't","아포스트로피 누락"},
        {"dont","don't","아포스트로피 누락"}, {"cant","can't","아포스트로피 누락"},
        {"doesnt","doesn't","아포스트로피 누락"}, {"didnt","didn't","아포스트로피 누락"},
        {"isnt","isn't","아포스트로피 누락"}, {"wasnt","wasn't","아포스트로피 누락"},
        {"couldnt","couldn't","아포스트로피 누락"}, {"shouldnt","shouldn't","아포스트로피 누락"},
        {"im","I'm","아포스트로피 누락"}, {"ive","I've","아포스트로피 누락"},
        {"youre","you're","아포스트로피 누락"}, {"theyre","they're","아포스트로피 누락"},
        {"thats","that's","아포스트로피 누락"},
        {"iam","I am","띄어쓰기 누락: 'I am'"},
    };
    return v;
}

const std::vector<Phrase>& phrases() {
    static const std::vector<Phrase> v = {
        {"could of","could have","'of'가 아니라 'have'를 써야 합니다"},
        {"should of","should have","'of'가 아니라 'have'를 써야 합니다"},
        {"would of","would have","'of'가 아니라 'have'를 써야 합니다"},
        {"must of","must have","'of'가 아니라 'have'를 써야 합니다"},
        {"might of","might have","'of'가 아니라 'have'를 써야 합니다"},
        {"your welcome","you're welcome","'you are'의 축약형 'you're'가 필요합니다"},
        {"their is","there is","'there is'가 올바릅니다"},
        {"their are","there are","'there are'가 올바릅니다"},
        {"there going","they're going","'they're'(they are)가 필요합니다"},
        {"he don't","he doesn't","3인칭 단수 주어에는 'doesn't'를 씁니다"},
        {"she don't","she doesn't","3인칭 단수 주어에는 'doesn't'를 씁니다"},
        {"it don't","it doesn't","3인칭 단수 주어에는 'doesn't'를 씁니다"},
        {"i is","I am","주어-동사 수일치: 'I am'"},
        {"i are","I am","주어-동사 수일치: 'I am'"},
        {"he have","he has","주어-동사 수일치: 'he has'"},
        {"she have","she has","주어-동사 수일치: 'she has'"},
        {"it have","it has","주어-동사 수일치: 'it has'"},
        {"everyone have","everyone has","'everyone'은 단수 취급합니다"},
        {"everybody have","everybody has","'everybody'는 단수 취급합니다"},
        {"they is","they are","주어-동사 수일치: 'they are'"},
        {"we is","we are","주어-동사 수일치: 'we are'"},
        {"you is","you are","주어-동사 수일치: 'you are'"},
        {"more better","better","비교급 중복"},
        {"more easier","easier","비교급 중복"},
        {"more faster","faster","비교급 중복"},
        {"most best","best","최상급 중복"},
        {"between you and i","between you and me","전치사 뒤에는 목적격 'me'를 씁니다"},
        {"loose weight","lose weight","'lose'(잃다)가 맞습니다"},
        {"for all intensive purposes","for all intents and purposes","관용구 오류"},
        {"to much","too much","'too'(너무)가 맞습니다"},
        {"to many","too many","'too'(너무)가 맞습니다"},
        {"better then","better than","비교 표현에는 'than'을 씁니다"},
        {"more then","more than","비교 표현에는 'than'을 씁니다"},
        {"less then","less than","비교 표현에는 'than'을 씁니다"},
        {"rather then","rather than","'rather than'이 올바릅니다"},
        {"other then","other than","'other than'이 올바릅니다"},
        {"bigger then","bigger than","비교 표현에는 'than'을 씁니다"},
        {"smaller then","smaller than","비교 표현에는 'than'을 씁니다"},
        {"faster then","faster than","비교 표현에는 'than'을 씁니다"},
        {"its a","it's a","'it is'의 축약형 'it's'가 필요합니다"},
        {"its an","it's an","'it is'의 축약형 'it's'가 필요합니다"},
        {"its the","it's the","'it is'의 축약형 'it's'가 필요합니다"},
        {"its not","it's not","'it is'의 축약형 'it's'가 필요합니다"},
        {"its been","it's been","'it has'의 축약형 'it's'가 필요합니다"},
        {"its very","it's very","'it is'의 축약형 'it's'가 필요합니다"},
        {"i am agree","I agree","'agree'는 동사이므로 'am'이 필요 없습니다"},
        {"discuss about","discuss","'discuss'는 타동사라 'about'이 필요 없습니다"},
        {"depend of","depend on","'depend on'이 올바릅니다"},
        {"an other","another","'another'는 한 단어입니다"},
    };
    return v;
}

const std::vector<const char*>& contractions() {
    static const std::vector<const char*> v = {
        "don't","doesn't","didn't","can't","won't","isn't","aren't","wasn't","weren't",
        "haven't","hasn't","hadn't","wouldn't","couldn't","shouldn't","mustn't","needn't",
        "i'm","you're","he's","she's","it's","we're","they're","i've","you've","we've",
        "they've","i'll","you'll","he'll","she'll","we'll","they'll","i'd","you'd","he'd",
        "she'd","we'd","they'd","that's","there's","what's","let's","who's","here's",
        "where's","o'clock","y'all","ain't","ma'am",
    };
    return v;
}

}  // namespace sc::en
