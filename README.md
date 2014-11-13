GoogleTranslateApi
==================

Reverse engineering of the Google Website Translator gadget's api
The gadget uses HTTP GET Method to fech data from the server

Base Url: https://translate.google.com/translate_a/single
Example query: ?client=t&sl=en&tl=fr&hl=en&dt=bd&dt=ex&dt=ld&dt=md&dt=qc&dt=rw&dt=rm&dt=ss&dt=t&dt=at&dt=sw&q=Hello

The format of the Translate's reply is described using the BNF-syntax
see  GoogleWebTranslateFormat.txt for reference
