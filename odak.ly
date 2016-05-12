normal = \new Staff { 
fis''4~ fis''8. r8 fis''4~ fis''8. ( fis''16 ) r8 g''2 r8 a''8. ( a''4. ) gis''16 r2 r16 
}
bass = \new Staff { 
\clef "bass" 

}
{
\new PianoStaff << 
\normal 
\bass 
>> 
} 
