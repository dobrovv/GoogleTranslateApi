GoogleTranslateApi
==================

Reverse engineering of the Google Website Translator gadget's api

The gadget uses HTTP GET Method to fech data from the server

```
Base Url: https://translate.google.com/translate_a/single
Example query: ?client=t&sl=en&tl=fr&hl=en&dt=bd&dt=ex&dt=ld&dt=md&dt=qc&dt=rw&dt=rm&dt=ss&dt=t&dt=at&dt=sw&q=Hello
```

The format of the Translate's reply is described using the BNF-syntax.

see [GoogleWebTranslateFormat.txt](https://github.com/VaSaKed/GoogleTranslateApi/blob/master/GoogleWebTranslateFormat.txt) for additional examples

```
<HttpGetReply> ::=
[
 [<Translation>*, <Translit>],
 [<PosDict>*],
 lang_source_abrev,            // iso639-1 source language code (ex. en | fr)
 ?,
 ?(!),
 ?(!),
 ?,
 ?,
 ?(!),
 ?,
 ?,
 [<SynonymDict>*],
 [<DefineDict>*],
 [<ExampleDict>],
 [<SeeAlsoDict>],
 server_time?
]

<Translation> ::= [         // Pair of the translated sentence and it's original
    sentence_target,
    sentence_source
]

<Translit> ::= [            // Transliteration of the translated text and original
    ?,
    ?,
    translit_target,
    translit_source
]

<PosDict> ::= [             // Dictionary that contains Part's Of the Speech, word transl and retransl
    pos_name_hl,
    [word_target*],
    [<PosDictEntry>*],
    word_source_base_form,  // infinitif
    <PosDictEnum>           // associates pos_name_hl with a number
]

<PosDictEntry> ::= [
    word_target,
    [word_retranslated*],      // list of retranslations
    [synset_id*],              // list of num's
    score,                     // how common the word is
    previous_word_target       // le article - lol :D
]

<SynDict> ::= [             // Dictionary that contains synonyms of the source word
    pos_name,               // can be empty
    [<SynDictEntry>*],
    word_base_form
]

<SynDictEntry> :: = [
    [synonym*],             // list of synonyms
    word_id                 // string to identify the word, used also in DefDict and ExapleDict
]

<DefDict> ::= [             // Dictionary that contains Definitions of the source word
    pos_name,               // can be empty
    [<DefDictEntry>*],
    word_base_form
]

<DefDictEntry> ::= [
    definition,
    word_id,
    definition_example_usage
]

<ExampleDict> ::= [         // Dictionary that contains usage Examples of the source word
    [<ExampleDictEntry>*]
]

<ExampleDictEntry> ::= [
    example,
    ?,
    ?,
    ?,
    ?-num,
    word_id
]

<SeeAlsoDict> ::= [
    [SeeAlso*]              // list of suggestions to see
]

<PosDictEnum> ::=          // an enum of possible part-of-speech for PosDict
    1 |     // Noun
    2 |     // Verb
    3 |     // Adjective
    5 |     // Preposition
    6 |     // Abbreviation
    7 |     // Conjunction
    8 |     // Pronoun
    9 |     // Interjection
   10       // Phrase

```
