# Descriere Task-urilor

	In aceasta tema am avut de simulat functionarea unei statii de incarcare pentru vehicule electrice, folosind multiple LED-uri si butoane.
	
## Descriere generala a functionarii:

	Initial statia de incarcare este libera. LED-urile din loader sunt stinse, iar LED-ul RGB are culoarea verde, ceea ce reprezinta disponibilitatea de a  incepe incarcarea.
	La apasarea scurta a butonului de start, LED-ul RGB isi schimba culoarea in rosu, pentru a arata ca statia este ocupata. Tot atunci incepe functia de incarcare, care utilizeaza LED-urile albastre. Se aprinde intai LED1, care clipeste 3 secunde, reprezentand 25% din procesul de incarcare. LED1 ramane aprins, iar apoi pentru alte 3 secunde, pana la 50% din incarcare LED2 clipeste. Intre 50% si 75% din incarcare LED3 clipeste, iar LED1 si LED2 raman aprinse, iar intre 75% si 100% din incarcare LED1, LED2 si LED3 sunt aprinse, iar LED4 clipeste.
	La sfarsitul procesului de incarcare, toate cele 4 LED-uri clipesc de 3 ori, dupa care se sting. LED-ul RGB devine verde pentru a 
arata ca statia este din nou libera.
	Daca butonul de start este apasat oricand in timpul incarcarii, nu se intampla nimic.
	Daca butonul de end este apasat lung (mai mult de 1 secunda) in timpul incarcarii, atunci cele 4 LED-uri ale loader-ului vor clipii
de 3 ori, dupa care se vor stinge, iar LED-ul RGB se va face verde.

## Organizarea Task-urilor:
	Task 1: Starea statiei – Disponibilitate si Indicatori
	In primul task, trebuie sa gestionam indicatorul de stare al statiei de incarcare, reprezentat de un LED RGB. Acesta va semnaliza 	daca statia este disponibila sau ocupata. Cand statia este libera, LED-ul va lumina in verde, iar cand statia este utilizata 	pentru incarcare, se va schimba in rosu. Prin aceasta abordare, putem identifica vizual starea statiei în orice	moment.

	Task 2: Simularea procesului de incarcare a bateriei
	Pentru simularea progresului de incarcare al unui vehicul electric, vor fi folosite patru LED-uri. Fiecare LED reprezinta un 	anumit procent din incarcare: 25%, 50%, 75% si 100%. Pe masura ce bateria se incarca, LED-urile se aprind succesiv la un interval 	de 3 secunde. LED-ul care indica procentul curent va clipi, in timp ce celelalte raman aprinse constant, reflectand nivelul deja 	incarcat. LED-urile ce urmeaza procentului curent raman stinse pana cand incarcarea ajunge la acel stadiu.

	Task 3: Initierea incarcarii
	Unul dintre butoanele disponibile are rolul de a porni procesul de incarcare. Dupa o apasare scurta a acestui buton, procesul de 	incarcare va incepe, iar LED-ul RGB va semnaliza faptul ca statia este ocupata, prin schimbarea culorii în rosu. Apasarea repetata 	a butonului in timpul incarcarii nu va avea niciun efect, prevenind orice interferente sau erori.

	Task 4: Oprirea si resetarea incarcarii
	Al doilea buton are functia de oprire si resetare a procesului de incarcare. Daca acesta este apasat si mentinut timp de cel putin 	1 secunda in timpul procesului de incarcare, statia va opri imediat procesul si va reveni la starea initiala de disponibilitate, 	cu LED-ul RGB schimbându-se din nou în verde. De asemenea, va rula o animatie in care toate LED-urile clipesc de 3 ori pentru a 	indica oprirea.

	Task 5: Debouncing-ul butoanelor
	Pentru a se asigura că actiunile declansate de butoane sunt corecte si nu sunt afectate de eventuale interferente electrice sau 	zgomote, este necesar sa se implementeze un mecanism de debouncing. Acesta asigura ca fiecare apasare de buton este inregistrata 	in mod corect, evitand inregistrarea multipla a unei singure apasari. Solutia implementata utilizeaza tehnici de debouncing 	software, ceea ce inseamna ca se introduce o logică suplimentara în cod pentru a filtra semnalele instabile de la butoane.
	
