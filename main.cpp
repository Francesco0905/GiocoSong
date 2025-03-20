#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <conio.h>  // Per Windows (kbhit)
#else
    #include <unistd.h>
    #include <sys/select.h>
#endif

using namespace std;

#define MAX_PLAYERS 10
#define MAX_ROUNDS 10
#define MAX_ATTEMPTS 3
#define TIME_LIMIT 30 // Tempo massimo per turno in secondi

const char* phrases[] = {
    "maurizio benazzi, è dal giorno zero che vivi succhiando cazzi",
"il rosso ti chiede di scoparlo perchè sa che sei ricchione",
"una severa quanto onesta punizione",
"il rosso si troverà presto un nuovo amante",
"uno come il benaz è difficile da trovare",
"ma ci devi ancora un mc, brutto pezzente",
"Ok ok, qui si parla di nuovo del ritardato",
"Quando morirai solo la apple sarà in lutto",
"Mauri mauri spendaccione",
"Maiale è certamente più appropriato",
"Poi metti il colpo in Canna e punta alla gola",
"Il tuo corpo dai corvi divorato",
"Posso dire con certezza che avevamo ragione",
"Dici che il rosso è un grande amante della figa",
"Il giorno del funerale non sarò dispiaciuto",
"Venderai il culo per non rimanere senza un tetto",
"Hanno sempre addosso tutti gli occhi",
"Al sabato sera esci con la tua puttana",
"Sei così grasso che morirai a quarant’anni infartato",
"Ormai è diventato un bersaglio mirato",
"Ci siamo rotto in culo, il giorno è arrivato",
"Quello che dice quanto sei micro dotato",
"I carabinieri ti troveranno già sezionato",
"Dopo il botto verrai coperto di sputo",
"Se ci avessi chiesto scusa ti saresti salvato",
"Per ammazzarti ho avuto mille motivi",
"E così siamo arrivati al giorno del giudizio",
"La sua testa finirà in questa busta",
"La spedizione parte, le armi sono affilate",
"Pensando a come faremo fuori quel ratto",
"Nemmeno dio lo potrà salvare",
"Ora è il momento di ammazzare",
"Apriamo la porta le luci sono spente",
"Sul suo volto compare un’espressione impaurita",
"Coglione non muoverti, ormai sei fottuto",
"Quella dove parlo della sua dipartita",
"Il ciccione è sul tavolo, ha smesso di lamentarsi",
"In questo modo non ci sarà rumore",
"Il battito è fermo, è proprio morto",
"Tanti sacchi neri per le parti di cadavere",
"Il peso è comunque difficile da smuovere",
"I genitori non denunceranno l’ accaduto"
// Canveduto ep
"Gli anni sono quaranta",
"Sei statale ma non è abbastanza",
"Sai bene che non si campa in eterno",
"So come funziona nelle bande",
"Il contatto è interessato l’accordo è preso",
"La pressione sembra infinita",
"Fa tutto per i soldi, è un uomo schifoso",
"Torni a casa e sembra tutto normale",
"Il viaggio dura poco, ecco la gang rivale",
"Smetti di pensare, agisci al volo",
"La vedi in vetrina, enorme e lucente",
"E dorme con la pistola temendo aggressioni",
//overdose
"È stato ucciso da droghe assassine",
"Roba pesante, le canne erano il meno",
"Fisico da sollevatore di grammi",
"Canny non c’è più, è morto di infarto",
"A te che a scuola mi davi del cancro",
"Il maresciallo si presenta, falso e cortese",
"La sniffata è decisa, la botta si sente",
"I minuti e i secondi, li aspetta al creatore",
"La commemorazione è quasi dantesca",
"Addio per sempre, alessandro canveduto",
    "Alex preparati questo pezzo è feroce",
    "Una dipartita assai veloce",
    "La droga la tua rovina",
    "Il frenci sippa un po di codeina",
    "Non credo proprio sia creatina",
    "Questa parte è solo per il drogato",
"Si vedono gli effetti delle sostanze",
"Ti perdi in una casa con due stanze",
"Hai sperperato tutte le finanze",
"Colpa della droga e delle circostanze",
"In famiglia sono queste le usanze",
"Tornare a casa ricoverati in ambulanze",
"Dopo un chiaro abuso di sostanze",
"Che altro dire se non condoglianze",

"Enzo e canny preparano la droga",
"Il frenci nella lean affoga",
"La tiene al sicuro nel baule",
"Putanna cucina ma non ha il grembiule",

"Enzo sappiamo che sei corrotto",
"A quanto pare anche il culo rotto",
"Il tiaccacci nelle vene a dirotto",
"Guarda come ti sei ridotto",
"Hai comprato quel motorino",
"E trasformato un figlio in fattorino",
"Da la merda ai raider in preferenziale",
"Per consegnare in fretta e guadagnare",
"A cazzeggiare a casa c’è canny Ale",
"Fai la cosa giusta e mettilo a lavorare",
"Così in ferie il frenci potrà andare",
"E la vita smettere di rischiare",

"Guidi il motorino da quando eri un minore",
"Aspetti il verde ma c’è uno strano odore",
"Non è la marmitta, il fumo viene dalla visiera",
"È proprio una canna o almeno lo era",
"Il semaforo è durato pochi secondi",
"Ma ecco che il mozzicone nascondi",
"Gli effetti ci sono, il mezzo sbanda",
"Porcodio si è schiantato a una serranda",
"Tornerai a casa col novanta sei",
"Ripensando al coglione che sei",

"La verifica è tra poco",
"Non hai capito niente",
"Matematica è difficile",
"Metti da parte l’odio",
"Chiedi se dopo scuola",
"ti spiego qualcosa",

"Che due coglioni, ma accetto",
"Forse mi sento in colpa per canny",
"Non sono un bulletto",
"Ma questa cosa durerà anni",
"Viene anche matte",
"La cosa non può far male",
"Quando durante la pausa",
"Compare una storia sul network sociale",
"Seiv canny hashtag pray for ale",
"Il nano esplode, ci vuole lasciare",
"Prende la roba e scende le scale",
"Ma porcodio dovevamo ripassare",

"Sandro palle si è offeso",
"Dio cane se sei peso",
"La storia era innocua",
"Non prenderla a male",
"Rimarrai per sempre spastico ale",
"Sandro cazzi se la prende",
"È meglio se non dici niente",
"Si offende instant se ti sente",
"È proprio un perdente",

"Corri verso la fermata",
"Ci hai fatto perdere la giornata",
"Ti sei portato l’acqua al ristorante",
"meglio se crepavi all’istante",
"La storia era simpatica",
"Una frase inedita",
"Ma ti sei offeso subito",
"Uomo dello spastolitico",
"Provo a chiederti scusa",
"Non vuoi sentire ragioni",
"Canny hai rotto i coglioni",
"La sessione ormai è conclusa",
"Sai cosa non finisce però? Esatto il bullismo",
"Complimenti ale, hai anche l’autismo",
"Dubbi non ce n’èrano Era evidente",
"Che eri down E pure pezzente",
"Ci vediamo domani in aula",
"Se non vieni preso dalla pula",

"Sandro palle si è offeso",
"Dio cane se sei peso",
"La storia era innocua",
"Non prenderla a male",
"Rimarrai per sempre spastico ale",
"Sandro cazzi se la prende",
"È meglio se non dici niente",
"Si offende instant se ti sente",
"È proprio un perdente",

"L’album è davvero pazzesco",
"Lo ascolto sempre quando esco",
"Alex ti farò fuori presto",
"Presta attenzione a questo testo",
"Dalla sala studio sei scappato",
"La storia insta ti ha blastato",
"Sul 96 ti sei impasticcato",
"Il bullismo non verrà dimenticato",
"Sandro palle merdomenico",
"Animale da palcoscenico",
"King delle sceneggiate napoletane",
"Se inviti la stupida in casa hai due puttane",

"Canny piange, è disperato",
"Si sente bullizzato",
"Ma il peggio non è passato",
"Wait till the album is droppato",

"Domenico è un merda!",
"Non è un opinione",
"Ma un teorema dimostrato",
"Con rigore e precisione",
"Il gay prostrato finisce ammazzato",

"Domenico spastico aka cannato",

"Merdale! Fatto di materia fecale",
"Alex merda! Aspetto che la vita perda",
"È un uomo di merda lo sanno tutti",
"Aspetta che il netturbino lo butti",
"Vive nel cesso essendo escremento",
"Tiro lo sciacquone vivo il momento",

"Domenico sterco bidone dell’immondizia",
"Falso e marcio come la sua amicizia",
"Simpatico come una malattia a stadio avanzato",
"Piangevi se poi ti chiamavo drogato",
"L’aeroplanino è atterrato con un messaggio",
"Mantenere 5 metri dalla fonte di drogaggio",
"Non me lo sono inventato, è un termine reale",
"Forse abbiamo più familiarità con spastico ale",
"Il concetto della song è che canny fa schifo",
"Se lo rapisce la mafia per l’esecuzione tifo",
"Merda d’uomo e d’amico, zero compleanni",
"Ah no aspetta, in un ci sono stati danni",
"Casini creati distruggendo una panca",
"Ma la voglia di bullismo è ancora tanta",

"Merdale! Fatto di materia fecale",
"Alex merda! Aspetto che la vita perda",
"È un uomo di merda lo sanno tutti",
"Aspetta che il netturbino lo butti",
"Vive nel cesso essendo escremento",
"Tiro lo sciacquone vivo il momento",

"Domenico è un merda!",
"Non è un opinione",
"Ma un teorema dimostrato",
"Con rigore e precisione",
"Il gay prostrato finisce ammazzato",

"Autistico ale che fa rime con merdale"

};
const int num_phrases = sizeof(phrases) / sizeof(phrases[0]);

struct Player {
    char name[50];
    int score;
};

void mask_last_word(const char* phrase, char* masked_phrase) {
    strcpy(masked_phrase, phrase);
    char* last_space = strrchr(masked_phrase, ' ');
    if (last_space) {
        last_space++;
        memset(last_space, '*', strlen(last_space));
    }
}

bool input_available() {
#ifdef _WIN32
    return _kbhit();
#else
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}

int main() {
    int num_players, num_rounds;
    Player players[MAX_PLAYERS];

    cout << "Inserisci il numero di giocatori: ";
    cin >> num_players;
    cin.ignore();  // Evita problemi con getline

    for (int i = 0; i < num_players; i++) {
        cout << "Inserisci il nome del giocatore " << i + 1 << ": ";
        cin.getline(players[i].name, 50);
        players[i].score = 0;
    }

    cout << "Inserisci il numero di round: ";
    cin >> num_rounds;
    cin.ignore();

    srand(time(0));

    for (int round = 0; round < num_rounds; round++) {
        for (int i = 0; i < num_players; i++) {
            cout << "\nRound " << round + 1 << " - Turno di " << players[i].name << "\n";

            int phrase_index = rand() % num_phrases;
            char masked_phrase[100];
            mask_last_word(phrases[phrase_index], masked_phrase);

            cout << "Frase: " << masked_phrase << "\n";

            char correct_word[50];
            sscanf(strrchr(phrases[phrase_index], ' ') + 1, "%s", correct_word);

            bool guessed = false;

            for (int attempt = 0; attempt < MAX_ATTEMPTS && !guessed; attempt++) {
                cout << "Tentativo " << attempt + 1 << " - Hai 30 secondi per rispondere!\n";

                time_t start = time(0);
                char user_guess[50] = "";
                bool time_expired = false;

                while (true) {
                    time_t now = time(0);

                    if (now - start >= TIME_LIMIT) {
                        cout << "\n⏳ Tempo scaduto! Nessun punto assegnato.\n";
                        guessed = true;
                        time_expired = true;
                        break;
                    }

                    if (input_available()) {
                        cin.getline(user_guess, 50);
                        if (strcmp(user_guess, correct_word) == 0) {
                            int points = (attempt == 0) ? 10 : (attempt == 1) ? 5 : 3;
                            players[i].score += points;
                            cout << "✅ Corretto! Hai guadagnato " << points << " punti.\n";
                            guessed = true;
                        } else {
                            cout << "❌ Sbagliato!\n";
                            if (attempt == 2) {
                                cout << "La parola era: " << correct_word << " ahahah gay\n";
                            }
                        }
                        break;
                    }

                    this_thread::sleep_for(chrono::milliseconds(100));
                }

                if (time_expired) break;
            }
        }

        cout << "\n🏆 Classifica dopo il round " << round + 1 << ":\n";
        for (int i = 0; i < num_players; i++) {
            cout << players[i].name << ": " << players[i].score << " punti\n";
        }
    }

    Player winner = players[0];
    for (int i = 1; i < num_players; i++) {
        if (players[i].score > winner.score) {
            winner = players[i];
        }
    }

    cout << "\n🏁 Classifica finale:\n";
    for (int i = 0; i < num_players; i++) {
        cout << players[i].name << ": " << players[i].score << " punti\n";
    }

    cout << "\n🎉 Il vincitore è: " << winner.name << " con " << winner.score << " punti!\n";
    cout << "Grazie per aver giocato!";
    return 0;
}