Textbaserad Tetris i C++

Sammanfattning:
Tetris är en serie av spel där det originella släpptes år 1985 av Alexey Pajitnov, en Soviets mjukvaruutvecklare. Det originella Tetris är detta projekt baserat på och spelet går i huvudsak ut på att sortera fallande block så att de täcker en hel rad av spelbrädet. Vid en full rad rensas den och poäng ges till spelaren. Tetris är ett av världens mest kända spel och trots dess ålder är spelet än idag relevant.

Bakgrund:
Projektet valdes p.ga den stora utmaning jag ansåg spelet skulle vara. Vilket jag ser som en fördel. Jag har fått träna på att konstruera spelbräden med hjälp av vektorer och for-loopar. Inmatning från spelaren är också ett huvudelement i spelet vilket har bidragit till mycket ny kunskap. Min felhanteringsfärdighet har definitivt utvecklats då i projektet filer både sparas och läses av. I vissa fall krävs till och med sortering av resultat i filerna vilket löstes med nya paket som går att importera till c++. Matematik krävdes också färdigheter vid fallen där tetrominos (ett block) ska roteras. Mer om det sen.

Nyckelaspekter:
Rotation: 

Hur används programmet?
Vid start möts du med en huvudmeny:

Här får du 5 olika val att välja mellan. 1 startar spelet. 2 låter användaren ändra svårighetsgrad, eller fallhastigheten. 3 visar highscore listan. 4 laddar ett sparat spel och 5 stänger av spelet.






Vid start möts du av denna vy: ![alt text]

Här får du spela spelet med din valda svårighetsgrad. Poäng visaren ses uppe i högra hörnet. Du får 100 poäng per rad rensad. Använd piltangenterna för att styra. 

‘⬆’ för rotera
‘⬅’ vänster
‘⮕’ höger
‘⬇’  dubbla hastighet ner
‘P’ pausa spelet



Vid tryck av ‘P’ pausas spelet och du möts då av detta:

Här kan du välja att spara spelet och återuppta det med samma värde, poäng och svårighetsgrad vid senare tillfälle. Du kan självklart också välja att gå tillbaka till menyn och fortsätta spela.




Vid Game Over ser du detta:

Du får här chansen att spara ditt resultat för att försöka nå topp 5 listan. Du kan också stänga av spelet eller gå tillbaka till menyn.






Utmaningar:
Spelet saknar förmågan att rotera vissa block nära väggarna. Kan fixas med det som kallas “Wall kicks”. I dagsläget kan man inte heller spara mer än ett spel (highscore kan du spara hur mycket som helst). Projektet är inte heller helt optimerat för alla enheter och skärmstorlekar/upplösningar. Den är i huvudsyfte byggd anpassad till min enhet och kan därav visas annorlunda på andra datorer. Detta går oftast att lösa genom att justera terminalfönstret på din enhet. 

Vad härnäst?:
Spelet kan utvecklas genom att implementera flera funktioner som finns i mer moderna tetris-spel. T.ex det som kallas ghost blocks. Där kan du se vart tetrominos skulle landa om du låter den falla snabbare. Det leder till den andra saken som kan göras. Snabbare fall eller direkta fall. 
