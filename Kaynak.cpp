#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
//oyun datas�
string bos="  ", w="w ", ww="ww", b="b ", bb="bb",tahta[8][8];
int whiteCount, blackCount;
bool whiteTurn;

void clr() {
	system("cls");
}
string encode(int r,int c) {
	//tahtadaki konum bilgisini a4b4 g�sterimine �evirir.
	string res="";
	res += (char)(c + 97);
	res += (char)((8 - r)+48);
	return res;
}
void decode(string s,int &r,int &c) {
	//a4b4 g�steriminde olan kodu tahtadaki konum bilgilerine �evirir. �rne�in:tahta[r][c]
	c = ((int)s[0]) - 97;
	r = 8-(((int)s[1]) - 48);
}
void writeFile(vector<string>& change) {
	//tahtadaki de�i�iklikleri dosyaya kaydeder.
	//e�er de�i�iklik yoksa hamle yap�lmam��t�r. oyun yeniden kuruluyor demektir.
	fstream file;
	if (!change.size()) {
		file.open("board.txt",ios::out);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 7; j++) {
				file << tahta[i][j] << "|";
			}
			file << tahta[i][7] << endl;
		}
	}
	else {
		file.open("board.txt", ios::in | ios::out);
		for (vector<string>::iterator it = change.begin(); it != change.end(); it++) {
			int r, c;
			decode((*it).substr(0,2),r,c);
			string val = (*it).substr(2);
			file.seekp(r*25+c*3,ios::beg);
			cout << file.tellp();
			file << val;
		}
		file.seekp(8*25);
	}
	file << (whiteTurn ? "w" : "b");
	file.close();
}
void setup() {
	//yeni oyun i�in ayarlamalar� yapar.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == 1 || i == 2)
				tahta[i][j] = b;
			else if (i == 5 || i == 6)
				tahta[i][j] = w;
			else
				tahta[i][j] = bos;
		}
	}
	whiteTurn = true;
	whiteCount = 16;
	blackCount = 16;
}
void readFile() {
	//son kay�ttaki tahta durumunu dosyadan �eker.
	fstream file("board.txt", ios::in);
	if (!file.is_open()) {
		setup();
		writeFile(*new vector<string>());
		remove("moves.txt");
		return;
	}
	blackCount = 0;
	whiteCount = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 7; j++) {
			getline(file,tahta[i][j],'|');
			if (tahta[i][j] == w || tahta[i][j] == ww)
				whiteCount++;
			else if (tahta[i][j] == b || tahta[i][j] == bb)
				blackCount++;
		}
		getline(file, tahta[i][7], '\n');
		if (tahta[i][7] == w || tahta[i][7] == ww)
			whiteCount++;
		else if (tahta[i][7] == b || tahta[i][7] == bb)
			blackCount++;
	}
	string plays;
	file >> plays;
	whiteTurn = plays == "w";
	
	file.close();
}
bool draw() {
	//ekran� �izer
	clr();
	for (int i = 0; i < 17; i++) {
		if (!(i % 2)) {
			cout << "  +----+----+----+----+----+----+----+----+";
		}
		else {
			cout << 8-(i/2) <<" | " << tahta[i / 2][0] 
				<< " | " << tahta[i / 2][1]
				<< " | " << tahta[i / 2][2]
				<< " | " << tahta[i / 2][3]
				<< " | " << tahta[i / 2][4]
				<< " | " << tahta[i / 2][5]
				<< " | " << tahta[i / 2][6]
				<< " | " << tahta[i / 2][7]
				<< " | ";
		}
		cout << endl;
	}
	cout << "    A    B    C    D    E    F    G    H   \n\n";
	return true;
}
void writeMove(string color,int sr,int sc,int dr,int dc) {
	//hamleleri dosyaya kaydeder.
	fstream file("moves.txt",ios::app|ios::out);
	file << color << ":" << (char)(sc + 97) << (char)((8 - sr) + 48) << (char)(dc + 97) <<(char)((8-dr)+48)<<endl;
	file.close();
}
bool isWhite(int r, int c) {
	//rc konumundaki ta� beyaz m�
	if (tahta[r][c] == w || tahta[r][c] == ww)
		return true;
	return false;
}
bool isDama(int r, int c) {
	//rc konumundaki ta� dama m�
	if (tahta[r][c] == ww || tahta[r][c] == bb)
		return true;
	return false;
}
bool isEmpty(int r,int c) {
	//r c konumundaki kare bo�mu
	if (tahta[r][c] == bos)
		return true;
	return false;
}
bool canTakeStamp(int r,int c) {
	//r,c pozisyonundaki ta� herhangi bir ta�� yiyebiliyormu kontrol eder.
	if (!isEmpty(r,c)) {
		if (isDama(r,c)) {
			if (r > 1) {
				for (int i = r - 1; i > 0; i--) {
					if (!isEmpty(i, c)) {
						if(isWhite(i, c) == isWhite(r, c))
							break;
						if (isEmpty(i - 1, c))
							return true;
						else
							break;
					}
				}
			}
			if (r < 6) {
				for (int i = r + 1; i <7; i++) {
					if (!isEmpty(i, c)) {
						if (isWhite(i, c) == isWhite(r, c))
							break;
						if (isEmpty(i + 1, c))
							return true;
						else
							break;
					}
				}
			}
			if (c > 1) {
				for (int i = c - 1; i > 0; i--) {
					if (!isEmpty(r, i)) {
						if (isWhite(r, i) == isWhite(r, c))
							break;
						if (isEmpty(r, i-1))
							return true;
						else
							break;
					}
				}
			}
			if (c < 6) {
				for (int i = c + 1; i < 7; i++) {
					if (!isEmpty(r, i)) {
						if (isWhite(r, i) == isWhite(r, c))
							break;
						if (isEmpty(r, i+1))
							return true;
						else
							break;
					}
				}
			}

		}
		else {
			if (r > 1 && whiteTurn && !isEmpty(r - 1, c) && isWhite(r - 1, c) != isWhite(r, c) && isEmpty(r - 2, c))
				return true;
			if (r < 6 && !whiteTurn && !isEmpty(r + 1, c) && isWhite(r + 1, c) != isWhite(r, c) && isEmpty(r + 2, c))
				return true;
			if (c > 1 && !isEmpty(r, c-1) && isWhite(r, c-1) != isWhite(r, c) && isEmpty(r, c-2))
				return true;
			if (c < 6 && !isEmpty(r, c+1) && isWhite(r, c+1) != isWhite(r, c) && isEmpty(r, c+2))
				return true;
		}
	}
	return false;
}
void findObligatoryMoves(int r, int c, vector<string> &obligatoryMoves, string& tempPath, int &moveCount, int &tempMoveCount,bool& isDama) {
	//dama olup olmad���na g�re rek�rsif olarak yap�labilecek hamleleri bulur.
	//maximum ta� alma kural�na g�re bulunan zorunlu hamleler obligatory moves i�erisine kaydedilir.
	//maximum al�nan ta� say�s� moveCount'da tutulur.Rek�rsif olarak ge�ici yenilen ta� say�s� tempMoveCount'da tutulur.
	//zorunlu olup olmad��� bilinmeyen ge�ici yol bilgisi tempPath'de tutulur.
	//r,c ta��n o anki pozisyonunu g�sterir.
	//oynanan ta� s�rekli tahtaya eklenmedi�i i�in fonksiyon �ncesi kald�r�ld��� i�in dama olup olmad��� bilgisi isDama'da tutulur.
	
	//ta��n hareketine g�re yenilebilecek ta�lar� bulur ve olas� hamlelere g�re yenilme halinde
	//rek�rsif olarak �a��r�r.
	if (isDama) {
		if (r > 1) {
			for (int i = r - 1; i > 0; i--) {
				if (!isEmpty(i, c)) {
					if (isWhite(i, c) == whiteTurn)
						break;
					for (int j = i - 1; j >= 0; j--) {
						if (isEmpty(j, c))
						{
							//yenilen ta� ge�ici olarak kald�r�l�r
							string alinan = tahta[i][c];
							tahta[i][c] = bos;
							//ta��n ilk konumundaysa yeni girilen yol tempPath'e kaydedilir.
							if(!tempMoveCount)
								tempPath=encode(r,c)+encode(j,c);
							tempMoveCount++;
							findObligatoryMoves(j,c,obligatoryMoves,tempPath,moveCount,tempMoveCount,isDama);
							tahta[i][c] = alinan;
						}
						else
							break;
					}
					break;
				}
			}
		}
		if (r < 6) {
			for (int i = r + 1; i < 7; i++) {
				if (!isEmpty(i, c)) {
					if (isWhite(i, c) == whiteTurn)
						break;
					for (int j = i + 1; j <= 7; j++) {
						if (isEmpty(j, c))
						{
							string alinan = tahta[i][c];
							tahta[i][c] = bos;
							if (!tempMoveCount)
							tempPath=encode(r, c) + encode(j, c);
							tempMoveCount++;
							findObligatoryMoves(j, c, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
							tahta[i][c] = alinan;
						}
						else
							break;
					}
					break;
				}
			}
		}
		if (c > 1) {
			for (int i = c - 1; i > 0; i--) {
				if (!isEmpty(r, i)) {
					if (isWhite(r, i) == whiteTurn)
						break;
					for (int j = i - 1; j >= 0; j--) {
						if (isEmpty(r, j))
						{
							string alinan = tahta[r][i];
							tahta[r][i] = bos;
							if (!tempMoveCount)
							tempPath=encode(r, c) + encode(r, j);
							tempMoveCount++;
							findObligatoryMoves(r, j, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
							tahta[r][i] = alinan;
						}
						else
							break;
					}
					break;
				}
			}
		}
		if (c < 6) {
			for (int i = c + 1; i < 7; i++) {
				if (!isEmpty(r, i)) {
					if (isWhite(r, i) == whiteTurn)
						break;
					for (int j = i + 1; j <= 7; j++) {
						if (isEmpty(r, j))
						{
							string alinan = tahta[r][i];
							tahta[r][i] = bos;
							if (!tempMoveCount)
							tempPath=encode(r, c) + encode(r, j);
							tempMoveCount++;
							findObligatoryMoves(r, j, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
							tahta[r][i] = alinan;
						}
						else
							break;
					}
					break;
				}
			}
		}

	}
	else {
		if (r > 1 && whiteTurn && !isEmpty(r - 1, c) && isWhite(r - 1, c) != whiteTurn && isEmpty(r - 2, c)) {
			string alinan = tahta[r-1][c];
			tahta[r-1][c] = bos;
			if (!tempMoveCount)
			tempPath=encode(r, c) + encode(r-2, c);
			tempMoveCount++;
			findObligatoryMoves(r-2, c, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
			tahta[r-1][c] = alinan;
		}
		if (r < 6 && !whiteTurn && !isEmpty(r + 1, c) && isWhite(r + 1, c) != whiteTurn && isEmpty(r + 2, c)) {
			string alinan = tahta[r + 1][c];
			tahta[r + 1][c] = bos;
			if (!tempMoveCount)
			tempPath=encode(r, c) + encode(r+2, c);
			tempMoveCount++;
			findObligatoryMoves(r + 2, c, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
			tahta[r + 1][c] = alinan;
		}
		if (c > 1 && !isEmpty(r, c - 1) && isWhite(r, c - 1) != whiteTurn && isEmpty(r, c - 2)) {
			string alinan = tahta[r][c-1];
			tahta[r][c-1] = bos;
			if (!tempMoveCount)
			tempPath=encode(r, c) + encode(r, c-2);
			tempMoveCount++;
			findObligatoryMoves(r, c-2, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
			tahta[r][c-1] = alinan;
		}
		if (c < 6 && !isEmpty(r, c + 1) && isWhite(r, c + 1) != whiteTurn && isEmpty(r, c + 2)) {
			string alinan = tahta[r][c + 1];
			tahta[r][c + 1] = bos;
			if (!tempMoveCount)
			tempPath=encode(r, c) + encode(r,c+2);
			tempMoveCount++;
			findObligatoryMoves(r, c + 2, obligatoryMoves,tempPath, moveCount, tempMoveCount,isDama);
			tahta[r][c + 1] = alinan;
		}
	}
	//t�m olas� hamlelere girdikten sonra bu sat�ra geldi�inde son pozisyon i�in olas� t�m yemeler uygulanm�� demektir.
	//ge�ici ta� yeme say�s� �uana kadar tuttugumuz zorunlu hamlelerin ta� yeme say�s�ndan fazlaysa
	//bulunan zorunlu hamleler ge�ersizdir ve onlar� siler ve en son bulunan path'i zorunlu hamlelere ekler.
	if (tempMoveCount > moveCount) {
		obligatoryMoves.clear();
		obligatoryMoves.push_back(tempPath);
		moveCount = tempMoveCount;
	}
	//e�er bulunan yolda mevcut olan max ta� yeme say�s� ile ayn� say�da ta� yerse
	//ve zorunlu hamleler i�erisinde bu path yoksa bu path'i de ekler.
	else if (tempMoveCount!=0 && tempMoveCount == moveCount && find(obligatoryMoves.begin(),obligatoryMoves.end(),tempPath)==obligatoryMoves.end())
		obligatoryMoves.push_back(tempPath);
	//yenilen ta�� geri yerine koyar
	tempMoveCount--;
}
bool checkCommand(string &input, bool& improperInput, bool &lastMove) {
	//girdide hata olu�ursa default de�erler
	improperInput = true;
	lastMove = false;
	
	//komut 4 karakter olmal�
	if (input.length() != 4) {
		input = "Command have to have 4 character!";
		return false;
	}
	//girdiyi okumay� kolayla�t�rma amac�yla k���lt�ld�
	for (int i = 0; i < input.length(); i++)
		input[i] = tolower(input[i]);

	//ana men�ye d�nmek i�in
	if (input == "menu")
		return true;

	//girilen inputtaki konumlar� orjinal tahta konumlar�na �evirir.
	int sr, sc, dr, dc;
	for (int i = 0; i < 3; i += 2)
		switch (input[i]) {
		case 'a':
			if (i == 0)
				sc = 0;
			else
				dc = 0;
			break;
		case 'b':
			if (i == 0)
				sc = 1;
			else
				dc = 1;
			break;
		case 'c':
			if (i == 0)
				sc = 2;
			else
				dc = 2;
			break;
		case 'd':
			if (i == 0)
				sc = 3;
			else
				dc = 3;
			break;
		case 'e':
			if (i == 0)
				sc = 4;
			else
				dc = 4;
			break;
		case 'f':
			if (i == 0)
				sc = 5;
			else
				dc = 5;
			break;
		case 'g':
			if (i == 0)
				sc = 6;
			else
				dc = 6;
			break;
		case 'h':
			if (i == 0)
				sc = 7;
			else
				dc = 7;
			break;
		default:
			input = "Wrong Column!";
			return false;
		}
	for (int i = 1; i < 4; i += 2)
		switch (input[i]) {
		case '1':
			if (i == 1)
				sr = 7;
			else
				dr = 7;
			break;
		case '2':
			if (i == 1)
				sr = 6;
			else
				dr = 6;
			break;

		case '3':
			if (i == 1)
				sr = 5;
			else
				dr = 5;
			break;

		case '4':
			if (i == 1)
				sr = 4;
			else
				dr = 4;
			break;

		case '5':
			if (i == 1)
				sr = 3;
			else
				dr = 3;
			break;

		case '6':
			if (i == 1)
				sr = 2;
			else
				dr = 2;
			break;

		case '7':
			if (i == 1)
				sr = 1;
			else
				dr = 1;
			break;
		case '8':
			if (i == 1)
				sr = 0;
			else
				dr = 0;
			break;

		default:
			input = "Wrong Row!";
			return false;
		}

	//bo� kareyi veya rakip ta�� oynat�p oynatmad���n� kontrol eder
	if (whiteTurn) {
		if (isEmpty(sr, sc)) {
			input = "White has to move a white stamp";
			return false;
		}
		else if (!isWhite(sr, sc)) {
			input = "White can't move black's stamp !";
			return false;
		}
	}
	else {
		if (isEmpty(sr, sc)) {
			input = "Black has to move a black stamp";
			return false;
		}
		else if (isWhite(sr, sc)) {
			input = "Black can't move white's stamp !";
			return false;
		}
	}

	//dolu kareye hamle yap�l�p yap�lmad���n� kontrol eder
	if (!isEmpty(dr,dc)) {
		input = "Can't move to the not empty tile!";
		return false;
	}

	//oynanan ta��n bulundu�u yere hamle yap�l�p yap�lmad���n� kontrol eder.
	if (sr == dr && sc == dc) {
		input = "The stamp cant move to same tile where it has been!";
		return false;
	}
	//�apraz hareketi kontrol eder
	if (sr != dr && sc != dc) {
		input = "Stamp can't move except horizontal or vertical line!";
		return false;
	}
	
	vector<string> obligatoryMoves;
	string tempPath="";
	int moveCount=0,tempMoveCount=0;
	bool isDam;
	//son hamleyi �eker
	fstream file("moves.txt", ios::in|ios::ate);
	file.seekg(-8 + file.tellg());
	string lastMoveRec;
	getline(file, lastMoveRec);
	file.close();

	//Bu k�s�m zorunlu hamleleri kontrol eder.
	//e�er ta� yenmi�se son hamlenin s�ras� ile �imdiki s�ra ayn�d�r.
	//Ve e�er ta� yenmi�se en son oynanan ta� oynanmak zorundad�r.
	//Bu sebeple di�er ta�lar i�in zorunlu hamleleri kontrol etmez.
	if ((lastMoveRec[0]=='w' && whiteTurn)||(lastMoveRec[0]=='b' && !whiteTurn)) {
		int rr, cc;
		decode(lastMoveRec.substr(4, 2),rr,cc);
		isDam = isDama(rr,cc);
		//dama, ta�lar� yerken rr,cc pozisyonundan ta� kalkaca�� i�in ta�� kald�r�yoruz ge�ici olarak.
		//dama de�ilse bulundu�u konuma tekrar gelemeyecektir zaten �ak��ma olmaz.
		string temp = tahta[rr][cc];
		if (isDam)
			tahta[rr][cc] = bos;
		findObligatoryMoves(rr, cc, obligatoryMoves, tempPath, moveCount, tempMoveCount,isDam);
		if (isDam)
			tahta[rr][cc] = temp;
	}
	//en son ta� yenmemi�se t�m ta�lar i�in zorunlu hamleleri hesaplar.
	else
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				//i,j konumundaki ta�, ta� yiyebiliyorsa zorunlu hamle vard�r
				if (canTakeStamp(i, j) && isWhite(i,j)==whiteTurn) {
					isDam = isDama(i,j);
					string temp = tahta[i][j];
					if (isDam)
						tahta[i][j] = bos;
					findObligatoryMoves(i,j,obligatoryMoves,tempPath,moveCount,tempMoveCount,isDam);
					tahta[i][j] = temp;
					tempMoveCount++;
				}
			}
		}

	//Olas� t�m zorunlu hamlelerle uyu�muyorsa son hamle hata verir.
	if (obligatoryMoves.size() && find(obligatoryMoves.begin(), obligatoryMoves.end(),input)==obligatoryMoves.end()) {
		input = "Obligatory moves are: ";
		for (vector<string>::iterator it = obligatoryMoves.begin(); it != obligatoryMoves.end(); it++)
			input += *it+" ";
		input += "!";
		return false;
	}

	//son oynayan rengi tutar
	string color=whiteTurn?"w":"b";
	//board.txt dosyas�nda yap�lacak de�i�iklikleri tutar
	vector<string> changes;
	
	//dama olup olmamas�na g�re tahtada yapt��� hamlede bir s�k�nt� yok ise hamleyi ger�ekle�tirir.
	if (isDama(sr,sc)) {
		bool isTaken = false;
		int takenRow, takenCol;
		if (sr == dr) {
			if (sc<dc) 
			{
				for (int i = sc + 1; i < dc; i++) {
					if (!isEmpty(sr, i)) {
						if (!isTaken) {
							isTaken = true;
							takenRow = sr;
							takenCol = i;
						}
						else {
							input = "You can jump over one stamp only!";
							return false;
						}
						if (isWhite(sr, i) == isWhite(sr, sc)) {
							input = "You can jump over opponent stamps only!";
							return false;
						}
					}
				}
			}
			else {
				for (int i = sc -1 ; i > dc; i--) {
					if (!isEmpty(sr, i)) {
						if (!isTaken) {
							isTaken = true;
							takenRow = sr;
							takenCol = i;
						}
						else {
							input = "You can jump over one stamp only!";
							return false;
						}
						if (isWhite(sr, i) == isWhite(sr, sc)) {
							input = "You can jump over opponent stamps only!";
							return false;
						}
					}
				}
			}
		}
		else  {
			if (sr < dr) {
				for (int i = sr + 1; i < dr; i++) {
					if (!isEmpty(i, sc)) {
						if (!isTaken) {
							isTaken = true;
							takenRow = i;
							takenCol = sc;
						}
						else {
							input = "You can jump over one stamp only!";
							return false;
						}
						if (isWhite(i, sc) == isWhite(sr, sc)) {
							input = "You can jump over opponent stamps only!";
							return false;
						}
					}
				}
			}
			else
			{
				for (int i = sr - 1; i > dr; i--) {
					if (!isEmpty(i, sc)) {
						if (!isTaken) {
							isTaken = true;
							takenRow = i;
							takenCol = sc;
						}
						else {
							input = "You can jump over one stamp only!";
							return false;
						}
						if (isWhite(i, sc) == isWhite(sr, sc)) {
							input = "You can jump over opponent stamps only!";
							return false;
						}
					}
				}
			}
		}
		tahta[dr][dc] = tahta[sr][sc];
		changes.push_back(encode(dr, dc) + tahta[sr][sc]);
		tahta[sr][sc] = bos;
		changes.push_back(encode(sr, sc) + bos);
		if (isTaken) {
			tahta[takenRow][takenCol] = bos;
			changes.push_back(encode(takenRow, takenCol) + bos);
			if (whiteTurn)
				blackCount--;
			else
				whiteCount--;
			if (!canTakeStamp(dr, dc))
				whiteTurn = !whiteTurn;
		}
		else
			whiteTurn = !whiteTurn;
	}
	else 
	{
	bool isTaken = false;
		if (sr == dr)
		{
			if (sc < dc) {
				if (sc + 2 == dc) {
					if (isEmpty(sr, sc + 1)) {
						input = "Stamps can move just one tile!";
						return false;
					}
					if (isWhite(sr, sc) == isWhite(sr, sc + 1)) {
						input = "You can jump over opponent stamps only!";
						return false;
					}
					isTaken = true;
					tahta[sr][sc + 1] = bos;
					changes.push_back(encode(sr, sc+1) + bos);
					if (whiteTurn)
						blackCount--;
					else
						whiteCount--;
				}
				
			}
			else {
				if (sc - 2 == dc) {
					if (isEmpty(sr, sc - 1)) {
						input = "Stamps can move just one tile!";
						return false;
					}
					if (isWhite(sr, sc) == isWhite(sr, sc - 1)) {
						input = "You can jump over opponent stamps only!";
						return false;
					}
					isTaken = true;
					tahta[sr][sc - 1] = bos;
					changes.push_back(encode(sr, sc-1) + bos);
					if (whiteTurn)
						blackCount--;
					else
						whiteCount--;
				}
			}
		}
		else {
			if (sr < dr) {
				if (sr + 2 == dr) {
					if (isEmpty(sr+1, sc)) {
						input = "Stamps can move just one tile!";
						return false;
					}
					if (isWhite(sr, sc) == isWhite(sr+1, sc)) {
						input = "You can jump over opponent stamps only!";
						return false;
					}
					isTaken = true;
					tahta[sr+1][sc] = bos;
					changes.push_back(encode(sr+1, sc) + bos);
					if (whiteTurn)
						blackCount--;
					else
						whiteCount--;
				}
				
			}
			else {
				if (sr - 2 == dr) {
					if (isEmpty(sr-1, sc)) {
						input = "Stamps can move just one tile!";
						return false;
					}
					if (isWhite(sr, sc) == isWhite(sr-1, sc)) {
						input = "You can jump over opponent stamps only!";
						return false;
					}
					isTaken = true;
					tahta[sr-1][sc] = bos;
					changes.push_back(encode(sr-1, sc) + bos);
					if (whiteTurn)
						blackCount--;
					else
						whiteCount--;
				}
			}
		}
		tahta[dr][dc] = tahta[sr][sc];
		tahta[sr][sc] = bos;
		changes.push_back(encode(sr, sc) + bos);
		if ((whiteTurn && dr == 0) || (!whiteTurn && dr == 7)) {
			if (whiteTurn)
				tahta[dr][dc] = ww;
			else
				tahta[dr][dc] = bb;
			whiteTurn = !whiteTurn;
		}
		else if (!isTaken)
			whiteTurn = !whiteTurn;
		else if (!canTakeStamp(dr, dc))
			whiteTurn = !whiteTurn;
		changes.push_back(encode(dr, dc) + tahta[dr][dc]);
	}
	

	//tahtada 1 ta� kald���nda damaya d�n��me kural�n� ger�ekle�tirir.
	if (whiteCount == 3) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (tahta[i][j] == w) {
					tahta[i][j] = ww;
					changes.push_back(encode(i, j) + ww);
				}
	}
	if (blackCount == 3) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (tahta[i][j] == b) {
					tahta[i][j] = bb;
					changes.push_back(encode(i,j) + bb);
				}
	}

	
	//hamle ger�ekle�ti flagleri ve dosya d�zenleme i�lemleri.
	improperInput = false;
	lastMove = true;
	writeMove(color,sr,sc,dr,dc);
	writeFile(changes);
	
	return true;
}
void logic() {
	string input="";
	bool improperInput= false;
	bool lastMove = false;
	//Men� yaz�l�ncaya kadar oyun komutlar�n� al�r.
	while (input!="menu") {
		//oyun bitimi
		if (whiteCount == blackCount && whiteCount == 1)
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (!isEmpty(i, j) && whiteTurn == isWhite(i, j) && !canTakeStamp(i, j)) {
						draw();
						cout << "GAME OVER! DRAW!\n\n";
						system("pause");
						return;
					}
		if (whiteCount == 0) {
			draw();
			cout << "GAME OVER! BLACK WINS!\n\n";
			system("pause");
			return;
		}

		else if (blackCount == 0) {
			draw();
			cout << "GAME OVER! WHITE WINS!\n\n";
			system("pause");
			return;
		}

		while (
			!draw() ||
			!(cout<< (improperInput?"Improper Command: "+input+"\n":"")) ||
			!(cout << (lastMove ? "Last Move: " + input + "\n" : "")) ||
			!(cout << (whiteTurn ? "White's " : "Black's ") << "Turn" << endl << "Command -> ") ||
			!(cin >> input) ||
			!checkCommand(input,improperInput,lastMove)
			);

	}
}

int main(int argc,char** argv) {
	string secenek = "";
	//ANA MEN�
	while (secenek!="3") {
		clr();
		cout << "1. New Game\n2. Continue Game\n3. End Game\n";
		//Girdi Kontrol�
		while (
			(!(cout << "Secenek -> ") ||
			!(cin >> secenek) ||
			secenek.length()!=1 ||
			secenek.find_first_not_of("123")!=string::npos) &&
			cout<<"\nImproper Input!\n"
			);
		//New Game
		if (secenek == "1") {
			setup();
			writeFile(*new vector<string>());
			remove("moves.txt");
			logic();
		}
		//Continue
		else if (secenek == "2"){
			readFile();
			logic();
		}
	}
}