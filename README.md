# Textbaserad Tetris i C++

Det här programmet är en textbaserad version av det klassiska spelet Tetris. Det körs i terminalen och låter spelaren styra fallande tetrominos, rensa rader och samla poäng.


Problemet som projektet löser är att det saknas enkla, textbaserade versioner av Tetris som både är spelbara och pedagogiska – särskilt i utbildningssyfte. Tetris är ett spel de flesta känner till, och att programmera det i C++ ger en bra övning i spelmekanik, logik, filhantering och användarinteraktion.
Min personliga motivation för att välja detta projekt är att jag gillar programmering som kombinerar kreativitet med logiskt tänkande. Det är dessutom ett roligt sätt att öva på filhantering, vektorer och menyer i C++. Tetris är ett spel jag själv uppskattar, vilket gjorde det ännu roligare att skapa en egen version.


Spara och ladda spel: Spelet kan sparas och laddas via textfiler, inklusive poäng, fallhastighet och brädets tillstånd.

Highscore-system: Efter game over kan användaren spara sitt resultat, som sparas i en fil och visas i topplista.

Färgkodning i terminalen: Olika färger visas beroende på tetrominotyp, vilket gör spelet mer visuellt trots att det är textbaserat.

Användning av _kbhit() och _getch(): Möjliggör realtidskontroller utan att behöva trycka Enter.

Meny med olika svårighetsgrader: Användaren kan välja mellan lätt, medel och svår.


Programmet körs genom att kompilera och starta filen. När spelet startar visas en meny där man kan välja att spela, ändra svårighet, visa highscore, ladda ett sparat spel eller avsluta.
När man spelar används piltangenterna:

Vänsterpil för att flytta vänster

Högerpil för att flytta höger

Nedåtpil för att snabba på fallet

Uppåtpil för att rotera tetrominon

'p' för att pausa spelet

Vid paus kan man spara spelet, återuppta det eller gå tillbaka till menyn. Om spelet avslutas (game over) får spelaren välja att spara poängen, gå till menyn eller avsluta helt.



Kollisionssystemet är grundläggande och kan t.ex. inte hantera "wall kicks" vid rotation nära väggar.

Spelet går bara att spela i terminalen på Windows (pga _kbhit() och _getch() från <conio.h>), vilket begränsar portabiliteten.

Det finns ingen nivåökning (spelet blir inte svårare ju längre du spelar).


För framtida utveckling skulle jag vilja:

Lägga till nivåökning där spelet automatiskt ökar hastigheten efter ett visst antal rader.

Göra spelet mer portabelt genom att ersätta <conio.h> med plattformsoberoende lösningar.

Implementera mer avancerad rotation som tillåter väggstuds (wall kicks).

Göra användargränssnittet ännu mer intuitivt, t.ex. genom att visa nästa tetromino.

Skapa en enklare inställningsfil där spelaren kan justera kontroller och färger.

