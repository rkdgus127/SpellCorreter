#!/usr/bin/env python3
"""hunspell en_US.dic/.aff 를 펼쳐(접사 적용) SpellCorreter용 dict/en_words.txt 를 만든다.
형식: "단어 빈도점수" (소문자). 빈도점수는 흔한 단어일수록 큼 (추천 후보 정렬에 사용).
사용: python3 tools/make_en_dict.py /usr/share/hunspell/en_US dict/en_words.txt
"""
import re, sys
base, out = sys.argv[1], sys.argv[2]

COMMON = """the be to of and a in that have i it for not on with he as you do at this but his by from they we say her she or an will my one all would there their what so up out if about who get which go me when make can like time no just him know take people into year your good some could them see other than then now look only come its over think also back after use two how our work first well way even new want because any these give day most us is are was were am been has had did does hello hi king love thank thanks please yes great little very much many here where why still should find tell ask need feel try leave call keep let begin seem help show hear play run move live believe bring happen write provide sit stand lose pay meet include continue set learn change lead understand watch follow stop create speak read allow add spend grow open walk win offer remember consider appear buy wait serve die send expect build stay fall cut reach kill remain name house world school state family student group country problem hand part place case week company system program question government number night point home water room mother area money story fact month lot right study book eye job word business issue side kind head far long big small large old young own same different important able early late high last next best better few more less those while between both each every never always often again off down through before under around three four five six seven eight nine ten friend father life man woman child thing something nothing everything someone anyone today tomorrow yesterday morning evening food car city door face door during without within against another such must might may shall hard easy happy sad free full real sure true false left once away together already enough almost really maybe perhaps spelling grammar check text sentence paragraph""".split()
rank = {w: i for i, w in enumerate(COMMON)}

# --- aff 파싱 ---
rules = {'PFX': {}, 'SFX': {}}
cross = {'PFX': {}, 'SFX': {}}
for line in open(base + '.aff', encoding='utf-8', errors='ignore'):
    p = line.split()
    if len(p) >= 4 and p[0] in ('PFX', 'SFX'):
        kind, flag = p[0], p[1]
        if len(p) == 4 and p[2] in ('Y', 'N'):          # 헤더
            cross[kind][flag] = (p[2] == 'Y'); rules[kind].setdefault(flag, [])
        elif len(p) >= 5:                                # 규칙
            strip = '' if p[2] == '0' else p[2]
            add = p[3].split('/')[0]; add = '' if add == '0' else add
            cond = p[4]
            rx = re.compile((cond + '$') if kind == 'SFX' else ('^' + cond)) if cond != '.' else None
            rules[kind].setdefault(flag, []).append((strip, add, rx))

def apply(word, kind, flag):
    res = []
    for strip, add, rx in rules[kind].get(flag, []):
        if rx and not rx.search(word): continue
        if kind == 'SFX':
            if strip and not word.endswith(strip): continue
            res.append(word[:len(word) - len(strip)] + add)
        else:
            if strip and not word.startswith(strip): continue
            res.append(add + word[len(strip):])
    return res

words = {}   # word -> score
def put(w, capitalized):
    if not re.fullmatch(r"[A-Za-z]+", w): return
    lw = w.lower()
    score = 1 if capitalized else 10            # 고유명사는 낮게
    if lw in rank: score = 1_000_000 - 1000 * rank[lw]
    if score > words.get(lw, 0): words[lw] = score

for i, line in enumerate(open(base + '.dic', encoding='utf-8', errors='ignore')):
    if i == 0: continue
    line = line.strip()
    if not line: continue
    w, _, flags = line.partition('/')
    w = w.split()[0] if w.split() else ''
    if not w: continue
    cap = w[0].isupper()
    put(w, cap)
    sfx = [f for f in flags if f in rules['SFX']]
    pfx = [f for f in flags if f in rules['PFX']]
    forms_s = [(x, f) for f in sfx for x in apply(w, 'SFX', f)]
    forms_p = [(x, f) for f in pfx for x in apply(w, 'PFX', f)]
    for x, _ in forms_s: put(x, cap)
    for x, _ in forms_p: put(x, cap)
    for xp, fp in forms_p:                               # 접두+접미 교차 결합
        if not cross['PFX'].get(fp): continue
        for xs, fs in forms_s:
            if cross['SFX'].get(fs):
                for y in apply(xs, 'PFX', fp): put(y, cap)

for w in COMMON: put(w, False)
with open(out, 'w', encoding='utf-8') as f:
    for w, s in sorted(words.items()):
        f.write(f"{w} {s}\n")
print(len(words), "words ->", out)
