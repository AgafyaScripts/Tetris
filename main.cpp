#include <iostream>
#include <vector> //Låter oss använda dynamiska arrayer
#include <ctime> //För srand
#include <cstdlib>
#include <thread>
#include <chrono>
#include <conio.h> //För att läsa av piltangenter
#include <fstream> //Öppna, skapa, redigera filer
#include <algorithm> //Bibliotek för att sortera toplistan i highscorre.txt

using namespace std;

const int width = 8; //Bredd och höjd på spelbrädet exklusive kanter
const int height = 14; 

bool fastFall = false; //Variabel för nedåtpilen. får tetrominon att falla snabbare
int score = 0;
bool gameOver = false;
bool paused = false;
int fallSpeed = 500; //Fallhastigheten/uppdateringshastigheten i ms. Sätt som standard på 500ms
vector<vector<int>> board; //För spelbrädet. en 2D vektor av typ där varje int är en "cell"
int currentColor = 1; //Integer för varje färg så drawBoard funktionen ska veta vilken tetromino som placerats ut och därefter vilken färg den ska vara

void clearScreen() {
    cout << "\033[2J\033[H"; //Uppdatera skärmen. Fick hjälp av klasskamrat med denna funktion
}

void showMenu(); //Funkions deklarationer
void startGame();
void spawnTetromino();
void drawBoard();
void updateGame();
bool loadGameState(vector<vector<int>>& board); //Slut på funktions deklarationer



void saveResult() { //Funktion för att spara resultat vid game over
    string name;
    cout << "\nEnter your name: ";
    cin >> name; //Få namn på spelaren
    ofstream file("highscores.txt", ios::app); //Öppna fil eller skapa om ej finns och lägg till data. ios::app för att inte radera gamal data
    if (file.is_open()) {//Kolla så filen öppnades
        file << name << " - " << score << endl; //Lägg till namn och poängen, avsluta med endl så ny rad görs nästa gång
        file.close(); //Stäng fil
        cout << "Result saved!\n";
    } else { //Om fil ej gick att öppna/skapa
        cout << "Could not save result.\n";
    }
}

void saveGameState(const vector<vector<int>>& board) { //Spara spel funktion. Tar med boarden just så den ser ut med hjälp av referens till det aktiva boardet
    ofstream file("gameState.txt"); //Öppna/skapa gameState.txt

    if (file.is_open()) {
        file << "Score: " << score << endl; //skriv score
        file << "Fall speed: " << fallSpeed << endl; //Svårighetgraden man valt eller fallhastigheten som det också kan kallas

        file << "Board:\n";
        for (int i = 0; i < height; i++) { //Skriv utt boardet rad för rad med mellanslag för tydlighetens skull
            for (int j = 0; j < width; j++) {
                file << board[i][j] << " ";
            }
            file << endl; //Ny rad
        }

        file.close();
        cout << "Game state saved!\n";
    } else {
        cout << "Could not save game state.\n"; //Ifall filen inte gick att öppna/skapa
    }
}

bool loadGameState(vector<vector<int>>& board) { //Bool fil för att ladda sparade spelet. Bool eftersom man ska kunna berätta om filen inte gick att öppna eller skapas
    ifstream file("gameState.txt"); //Öppna fil
    if (file.is_open()) {//Om det gick

        string line; //Sträng som tar emot rad för rad av filen
        getline(file, line); //Hitta första linen
        sscanf(line.c_str(), "Score: %d", &score); //sscanf för att läsa data från en sträng. Hitta heltalet efter Score:. %d heltal. Matcha datan med score
    
        getline(file, line);
        sscanf(line.c_str(), "Fall speed: %d", &fallSpeed);
    
        getline(file, line); 
    
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                file >> board[i][j]; //Söka igenom varje rad för att fylla brädet. Varje cell har en specifik bokstav som motsvarar en siffra. Mer om detta senare
            }
        }
    } else {
        cout << "Could not open game state file.\n";
        return false;
    }
    file.close();
    return true;
}

void showMenu() { //Funktionen som körs först
    ifstream file("highscores.txt");//Öppna highscore fil för eventuell visning
    vector<pair<int, string>> results; //Vektor där varje elemnt bestå av namn och poäng
    string line;
    while (getline(file, line)) { //Så länge det finns rader, läs in de
        int pos = line.find(" - "); //Letar efter vart i txt '-' finns
        if (pos != string::npos) { // om '-' hittades gör detta
            string name = line.substr(0, pos); //Plockar ut namnet
            string scoreText = line.substr(pos + 3); //Plockar ut score som befinner sig 3 steg efter där '-' stränge hittades. 
            int points = 0;
            sscanf(scoreText.c_str(), "%d", &points); //Tar scoreText som vi tidgare fått fram och  lägger till värdet i variabeln points
            results.push_back({points, name}); //Lägger till poäng + namn i vektorn results som deklarerats tidigare
        }
    } //Loopas tills alla poäng och namn lagts in i vektorn 
    file.close();
    sort(results.begin(), results.end(), [](auto &a, auto &b) { return a.first > b.first; }); //Sorterings funktion som kommer med biblioteket #include <alghoritm>. Jämför två poäng och ser vilken som är störst och rangordnar därefter. Sorterar helt enkelt vektorn
    clearScreen(); //Rensar skärmen för säkerhetskull
    cout << "===== TETRIS =====\n";//Visar menyn
    cout << "1. Play Tetris\n";
    cout << "2. Change difficulty\n";
    cout << "3. View Highscore\n";
    cout << "4. Load saved game\n";
    cout << "5. Exit\n";
    cout << "==================\n";
    cout << "Choice: ";

    char menuChoice;
    cin >> menuChoice;//Tar emot menyval
    if (menuChoice == '1') {
        startGame();//Startar spelet genom att gå till startgame funktion
    } else if (menuChoice == '2') {//Möjlighet att välja svårighet
        clearScreen();
        cout << "Choose difficulty:\n";
        cout << "1. Easy (1000ms)\n";
        cout << "2. Medium (500ms)\n";
        cout << "3. Hard (200ms)\n";
        cout << "Choice: ";
        char choice;
        cin >> choice;
        switch (choice) {
            case '1': fallSpeed = 1000; break;//Ändrar fallspeed beroende på vad man vill ha för svårighetsgrad
            case '2': fallSpeed = 500; break;
            case '3': fallSpeed = 200; break;
            default: fallSpeed = 500; break;
        }
        showMenu(); //Visar menyn på nytt/startar om funktionen
    } else if (menuChoice == '3') {
        clearScreen();
        cout << "Top 5 Highscores:\n"; //Ska visa highscore lista
        for (int i = 0; i < results.size() && i < 5; i++) { //Kör tills inga fler resultat finns eller 5 visats
            cout << i + 1 << ". " << results[i].second << " - " << results[i].first << " points\n"; //Skriver ut andra elementet först (namn) och sen points
        }
        cout << "Press enter to return to the menu...\n";
        cin.ignore(); //Rensar gammal inmatning
        cin.get();//Väntar på enter tangent
        showMenu(); 
    } else if (menuChoice == '4') {
        board = vector<vector<int>>(height, vector<int>(width, 0)); //Skapar ett tomt spelbräde med toma platser (0)
        if (loadGameState(board)) { //Anropar funktionen loadGameState för att återskapa bräde, svårighet och poäng
            gameOver = false; //Återställer värderna ifall du förlorat och därefter laddar spel. eller pausat
            paused = false;
            spawnTetromino(); //Skapar ny tetromino
            while (true) { //Forsätter tills break
                updateGame(); //Huvud game loopen som uppdaterar brädet konternuelit
                if (gameOver) break; //Om gameover = true. break loop
                this_thread::sleep_for(chrono::milliseconds(fallSpeed)); //Tid emmelan varje updatering. Svårighetsgraden
            }
        } else {
            cout << "Failed to load saved game. Returning to menu...\n"; //Om loadGameState inte fungerade så vänta 2 sek och sen återgå till spel
            this_thread::sleep_for(chrono::seconds(2));
            showMenu();
        }
    } else if (menuChoice == '5') { //Ifall man vill avsluta spelet (vilket jag hoppas ingen vill göra XD)
        exit(0); 
    }
}

struct Tetromino { //Struct för en Tetromino
    vector<vector<int>> shape; //2D vektor för formen. Varje bit på tetrominon eller cell har ett värde. En integer (1-7)
    int x, y; //Positionen för vart på spelbrädet tetrominon befinner sig i x och y led
};

Tetromino currentTetromino; //Variablen currentTetromino använder structen

vector<vector<vector<int>>> tetrominoes = { //En 2D vektor som håller alla olika tetrominos. Notera att de alla har olika nummer för att definiera en plats (0 är tom plats). Detta för att länka samman en viss siffra till en färg
    {{1, 1, 1, 1}}, //Valde att skriva I tetrominon i horisontell läge för att underlätta för spelaren och undvika snabb död
    {{2, 2, 2}, {0, 2, 0}},
    {{3, 3}, {3, 3}},
    {{4, 4, 0}, {0, 4, 4}},
    {{0, 5, 5}, {5, 5, 0}},
    {{6, 0, 0}, {6, 6, 6}},
    {{0, 0, 7}, {7, 7, 7}}
};

void drawBoard() { //Rita ut spelbrädet vid varje uppdatering
    clearScreen(); //Cleara tidigare brädet med funktion som Malte hjälpte mig med

    if (gameOver) { // Om du förlorat
        cout << "\n    GAME OVER!\n";
        cout << "    Final Score: " << score << "\n\n";
        cout << "1. Back to menu\n";
        cout << "2. Save result\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        char choice;
        cin >> choice;
        if (choice == '1') {
            showMenu();
        } 
        else if (choice == '2') { //Om spara resultat väljs kör saveResult funktinen och fråga om input igen
            saveResult();
            cout << "\n1. Back to menu\n2. Exit\nChoice: ";
            cin >> choice;
        }
        else {
            exit(0);
        }
        return;
    }

    if (paused) { //om spelet har pausats
        cout << "\n    PAUSED\n";
        cout << "1. Resume game\n";
        cout << "2. Save game state\n";
        cout << "3. Back to menu\n";
        char choice;
        cout << "Your choice: ";
        cin >> choice;

        if (choice == '1') {
            paused = false;
        }
        if (choice == '2') { // om spara spelet valt kör saveGameState funktion och skicka med det aktiva board
            saveGameState(board);
        } else if (choice == '3') {
            showMenu();
        }
        return;
    }

    vector<vector<int>> tempBoard = board; //Tillfällig kopia av board för att rita ut tetrominon
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) { //For loop till storleken på currentTetromino.shape för att få med hela tetrominon
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) { //Går rad för rad på den nuvarande slumpmässigt valda tetrominon
            if (currentTetromino.shape[i][j]) { //Kollar så att den valda rutan är en cell i tetrominon
                int y = currentTetromino.y + i; //Räknar ut y värdet på tetrominons cell
                int x = currentTetromino.x + j; //Räknar ut x värdet på tetrominons cell
                if (y >= 0 && y < height && x >= 0 && x < width) { //Checka så att tetrominons cell är inne i board
                    tempBoard[y][x] = currentColor; //Kolla vilken siffra cellen med tetrominon har så man senare vet vilken färg den ska vara
                }
            }
        }
    }

    cout << "\033[36m== TETRIS ==\033[0m" << "    Score: " << score << "\n"; // Visar title samt poäng

    for (int i = 0; i < height; i++) {
        cout << "\033[36m| \033[0m"; //Skriv ut vänster vägg på varje rad
        for (int j = 0; j < width; j++) { //Gå i x led och rita ut nåt i varje cell
            switch (tempBoard[i][j]) { //kolla siffran på cellen
                case 1: cout << "\033[36m#\033[0m"; break; //Om 1 så blir färgen cyan (ljusblå). O.s.v ner
                case 2: cout << "\033[35m#\033[0m"; break;
                case 3: cout << "\033[33m#\033[0m"; break;
                case 4: cout << "\033[31m#\033[0m"; break;
                case 5: cout << "\033[32m#\033[0m"; break;
                case 6: cout << "\033[34m#\033[0m"; break;
                case 7: cout << "\033[91m#\033[0m"; break;
                default: cout << "."; break; //Om ingen av ovanstående siffror är i cellen rita då ut en punkt. Valde att ha med punkter då detta underlättar med de visuella delarna
            }
        }
        cout << "\033[36m |\033[0m\n"; //Skriver ut höger vägg på varje rad
    }
    for (int i = 0; i < width + 4; i++) cout << "\033[36m=\033[0m"; //Skriver ut botten på spelbrädet
    cout << "\n";
    cout << flush; //Tvingar programmet att skriva ut alla cout här för att kunna uppdatera spelbrädet på ett smidigare sätt
}

bool canMove(int dx, int dy) { //Bool funktion som retunerar false eller true beroende på om tetrominon kan flyttas dit man vill
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) { //Loopar igenom varje cell i tetrominon
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j]) { //Kollar så att cellen är en del av tetrominon. Vissa delar av tetrominon är ju tomma rutor. T.ex i T tetrominon
                int newX = currentTetromino.x + j + dx; //Lägger till eventuell flyttning för cellen i x led
                int newY = currentTetromino.y + i + dy;//Och för gravitationen eller nedåtpil tryckning testas även y led
                if (newX < 0 || newX >= width || newY >= height || board[newY][newX]) //Kollar så att nya platsen för tetrominons cell är fri. board[newY][newX] kollar så att det är en 0:a i brädet och ingen annan tetromino redan ligger där
                    return false;
            }
        }
    }//Funktionen testar varje cell i tetrominon och returnerar false om platsen är upptagen eller utanför brädet
    return true;
}

void moveTetromino(int dx) { //Flyttar tetrominon
    if (canMove(dx, 0)) {//Kollar med canMove funktion ifall det går att flytta dx steg för varje cell i tetrominon
        currentTetromino.x += dx; //Om det funkar gör vi flytten
    }
}

void rotateTetromino() { //Funktion som aktiveras vid piltryckning uppåt
    vector<vector<int>> rotated; //Skapa ny vektor, rotated
    int rows = currentTetromino.shape.size(); //Tar fram rader resp kolumner på nuvarande tetrominon
    int cols = currentTetromino.shape[0].size();
    rotated.resize(cols, vector<int>(rows, 0)); //gör om vektor rotated där nya antal rader = gamla antal kolumner och tvärtom

    for (int i = 0; i < rows; i++) {//För varje rdad
        for (int j = 0; j < cols; j++) {//För varje cell
            rotated[j][rows - i - 1] = currentTetromino.shape[i][j]; //Läs mer om denna funktion i ReadMe filen
        }
    }

    vector<vector<int>> originalShape = currentTetromino.shape; //Spara orginalet innan rotation vid behov som vi snart får se
    currentTetromino.shape = rotated; //Gör om roterade tetrominon till nuvarande
    if (!canMove(0, 0)) { // Kollar om det går att flytta. Vi byter inget i x-led eller y-led. Om canMove retunerar false:
        currentTetromino.shape = originalShape; //Retunera till orginal shape
    }
}

void clearLines() { //Funktion som ska rensa rader
    int linesCleared = 0; //Integer för antal rader rensade
    for (int i = height - 1; i >= 0; i--) { //Loopar igenom alla rader nerifrån och upp
        bool fullLine = true; //Antar att rader är full
        for (int j = 0; j < width; j++) {//Loopar igenom varje cell på raden
            if (board[i][j] == 0) { //Om någon plats är 0/tom 
                fullLine = false;//sätt fullLine till false 
                break; //starta om på ny rad
            }
        }
        if (fullLine) {//Om ingen cell var tom
            linesCleared++;
            for (int k = i; k > 0; k--) {//Flytta ner varje rad ett steg genom att ändra varje rads y led. Börja från k = i så att endast allt ovanför den rensade raden blir nerflyttat 
                board[k] = board[k - 1]; //Gör så att raden k blir raden ovanför för alla rader ovanför k
            }
            board[0] = vector<int>(width, 0); //Bygg en ny rad längst upp
            i++; //Gå ner 1 rad igen för att kolla samma rad igen. Kan vara så att vi flyttade ner en rad ovanför första rensade raden som också var full. 
        }
    }
    score += linesCleared * 100; //Räkna ut hur mycket poäng som ska ges.
}

void lockPiece() {
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) {
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {//går igenom rad för rad och cell för cell
            if (currentTetromino.shape[i][j]) {
                int y = currentTetromino.y + i; 
                int x = currentTetromino.x + j;
                if (y >= 0 && y < height && x >= 0 && x < width) {//Kollar så cellerna är innanför brädet
                    board[y][x] = currentColor; //Ritar ut siffra i brädet som representerar en färg
                }
            }
        }
    }
    clearLines(); //Kolla om det finns lines att rensa efter varje lockPiece
}

bool collisionAtSpawn() { //Funktion som kollar ifall det är gameover. Den kikar om tetrominon kolliderar med nåt när den spwnas in
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) {
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j]) {
                int x = currentTetromino.x + j;
                int y = currentTetromino.y + i;
                if (y >= 0 && board[y][x]) return true; //Om tetrominon är inne i spelbrädet och board[y][x] inte är tom så retunera true
            }
        }
    }
    return false;
}

void spawnTetromino() {
    int index = rand() % tetrominoes.size(); //Väljer ut en slumpmässig tetromino ur tidigare definierad vektor. med hjälp av ssrand
    currentTetromino.shape = tetrominoes[index]; //Tar fram formen på tetrominon
    currentTetromino.x = width / 2 - 1; // sätt tetrominons koordinater till mitten längst upp
    currentTetromino.y = 0;
    currentColor = index + 1; //Färg beroende på vilken tetromino som valts. +1 för att vektorn börjar på 0

    if (collisionAtSpawn()) { //Kollar kollisioner när tetrominon sätts in
        gameOver = true; //Retunerar funktionen true blir det gameOver
    }
}

void handleInput() { //Funktion som kollar input
    if (_kbhit()) {//Har tangent tryckts ner. Om inte skippa funktionen
        int key = _getch(); //Ta emot tangent
        if (key == 224) {//Är det special tangent
            int arrow = _getch();//Kolla special tangent
            if (!paused) { //Kolla först så spelet inte är pausat
                if (arrow == 75) moveTetromino(-1);  //Vänster
                else if (arrow == 77) moveTetromino(1); //Höger
                else if (arrow == 80) fastFall = true; //Ner (aktiverar bool variabeln fastFall)
                else if (arrow == 72) rotateTetromino(); //Uppåt
            }
        } else if (key == 'p') { //Om det inte var speciatangent utan p
            paused = true; //Sätt paus till true
        }
        while (_kbhit()) _getch(); //Tömmer tangentbuffern
    }
}

void updateGame() { //HUvudspel loop
    handleInput(); //Kollar input

    if (gameOver || paused) { //Om gameOver eller paused aktiverat så:
        drawBoard();          //drawBoard. drawBoard kommer veta om spelet är över eller pausat
        return;
    }

    int fallSteps = fastFall ? 2 : 1; //Om fastFall aktiverat: neråt pil. åk två steg ner istället för ett
    fastFall = false; //Sätt fastFall till false igen 

    for (int i = 0; i < fallSteps; i++) { //Flytta block ner
        if (canMove(0, 1)) { //Ankallar canMove för att se att tetrominon kan flyttas
            currentTetromino.y++; //Om det går lägg till 1 på y värdet
        } else { //Om tetrominon inte kan falla
            lockPiece(); //Lås fast den
            spawnTetromino(); //spawna in en ny
            break;
        }
    }

    drawBoard(); //Måla ut brädet för att uppdatera 
}

void startGame() { //Starta tetris
    score = 0; //Nollställer alla variablar
    gameOver = false;
    paused = false;
    board = vector<vector<int>>(height, vector<int>(width, 0)); //Skapa en 2D vektor där alla värden är 0
    spawnTetromino(); //Spawna in första tetrominon
    while (true) { //Forsätt till break
        updateGame(); //Uppdatera spelet
        if (gameOver) break; //Om gameOver så stoppa spelet
        this_thread::sleep_for(chrono::milliseconds(fallSpeed)); //Tid emmelan varje uppdatering. För att ge dig tid att reagera på vad som händer
    }
}

int main() {
    srand(time(0)); //Gör en srand seed 
    showMenu();
    return 0;
}