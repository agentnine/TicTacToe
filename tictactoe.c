#include <stdio.h>
#include <stdbool.h>

typedef struct{
	char board[9];
}Tabel;

#define board(T)	(T).board
#define Elmt(T,i) 	(T).board[(i)]

typedef struct{
	Tabel awal[3];
	Tabel akhirMenang[200];
	Tabel akhirSeri[200];
	char simbol[9];
	Tabel state[250][10];
	int length;
} Papan;

typedef struct{
	Tabel state[100];
	int length;
} State;


void intro()
/*
	procedure akan menampilkan intro dari program
*/
{
	// KAMUS LOKAL
	char input;
	
	// ALGORITMA
	printf("################################################ TUGAS BESAR TBFO ################################################\n");
	printf("########################################### Juniardi Akbar / 13517075 ############################################\n");
	printf("##################################################################################################################\n");
	printf("###         ###         ####        ###         #####       ######        ###         ####       ####         ####\n");
	printf("######   #########   ######   ############   #######   ###   ####   ############   ######   ###   ###   ##########\n");
	printf("######   #########   ######   ############   #######   ###   ####   ############   ######   ###   ###   ##########\n");
	printf("######   #########   ######   ############   #######         ####   ############   ######   ###   ###        #####\n");
	printf("######   #########   ######   ############   #######   ###   ####   ############   ######   ###   ###   ##########\n");
	printf("######   #########   ######   ############   #######   ###   ####   ############   ######   ###   ###   ##########\n");
	printf("######   ######         ####        ######   #######   ###   #####        ######   #######       ####         ####\n");
	printf("##################################################################################################################\n");
	printf("\n");
	printf("################################################## TEKAN ENTER ###################################################\n");
	scanf("%c", &input);
	printf("#################################################### PETUNJUK ####################################################\n");
	printf("###################### PADA SETIAP GILIRAN BERMAIN, INPUT ANGKA DARI 1-9 UNTUK MENGISI PAPAN #####################\n");
	printf("#####################################  ADAPUN NOMOR UNTUK SETIAP PAPAN ADALAH ####################################\n");
	printf("                                                     1 | 2 | 3\n");
	printf("                                                     ---------\n");
	printf("                                                     4 | 5 | 6\n");
	printf("                                                     ---------\n");
	printf("                                                     7 | 8 | 9\n");	
	printf("\n");
	printf("################################################## TEKAN ENTER ###################################################\n");	
}

void bacaFile(Papan *P)
/* I.S. P sembarang (belum terisi)
	 F.S. P akan diisi setelah dibaca dari file eksternal	
*/
{
	// KAMUS LOKAL
	int i,j,k,l;
	Tabel scan;
	char scan2;
	
	FILE *in = fopen("state.txt", "r");
	//Membaca kumpulan state awal
	for (i=0; i<3; i++){
		fscanf(in, "%s", &scan); fflush(stdin);
		(*P).awal[i] = scan;
	}
	//Membaca kumpulan state akhir dengan keadaan komputer menang
	for (j=0; j<121; j++){
		fscanf(in, "%s", &scan); fflush(stdin);
		(*P).akhirMenang[j] = scan;
	}
	//Membaca kumpulan state akhir dengan keadaan komputer imbang
	for (k=0; k<14; k++){
		fscanf(in, "%s", &scan); fflush(stdin);
		(*P).akhirSeri[k] = scan;
	}
	//Membaca kumpulan simbol
	for (l=0; l<9; l++){
		fscanf(in, "%s", &scan2); fflush(stdin);
		(*P).simbol[l] = scan2;
	}
	//Membaca state dalam transition table dan membentuk menjadi matriks
	i=0;
	j=0;
	while (!feof(in)){
		fscanf(in, "%s", &scan); fflush(stdin);
		(*P).state[i][j] = scan;
		j++;
		if (j==10){
			j=0;
			i++;
		}
	}
	(*P).length = i;
	fclose(in);
	getchar();
}

void inisialisasi(Tabel *T)
/* I.S. Sembarang
	 F.S. Tabel sudah terisi dengan semua elemennya kosong ('#')
*/
{
	// KAMUS LOKAL
	int i;
	
	// ALGORITMA
	for (i=0; i<9; i++){
		Elmt(*T,i) = '#';
	}
}

void salinTabel(Tabel T1, Tabel *T2)
/* I.S. T1 sembarang
	 F.S. Tabel T1 disalin ke Tabel T2
*/
{
	// KAMUS LOKAL
	int i;
	
	// ALGORITMA
	for (i=0; i<9; i++){
		Elmt(*T2,i) = Elmt(T1,i);
	}
}

void tulisTabel(Tabel T)
/* I.S. T sembarang
	 F.S. Akan diabilkan tabel dalam bentuk papan permainan tic tac toe
*/
{
	// KAMUS LOKAL
	int i,j;
	
	// ALGORITMA
	for (i=0; i<3; i++){
		printf("                                                     ");
		for (j=(i*3); j<i*3+3; j++){
			if (j != i*3+2){
				if (Elmt(T,j) == '#')
					printf("%c | ", ' ');
				else
					printf("%c | ", Elmt(T,j));
			}
			else
				if (Elmt(T,j) == '#')
					printf("%c\n", ' ');
				else
					printf("%c\n", Elmt(T,j));
		}
		if (i!=2)
			printf("                                                     ----------\n");
	}
}

Tabel isiTabel(Tabel T, int i)
/* I.S. T sebarang
	 F.S. Elemen ke i pada T akan diisi dengan 'X'
*/
{
	Elmt(T,i) = 'X';
	return (T);
}

bool isSama(Tabel T1, Tabel T2)
/*
	 Mengembalikan true jka T1 dan T2 identik. false jika tidak. 
*/
{
	// KAMUS LOKAL
	int i;
	
	// ALGORITMA
	bool cek = true;
	for (i=0; i<9; i++){
		if (Elmt(T1,i) != Elmt(T2,i)){
			cek = false;
			break;
		}
	}
	return cek;
}

bool isMenang(Tabel T, Papan P)
/*
	 Mengembalikan true jika Tabel T ada di kumpulan state menang pada Papan.
*/
{
	// KAMUS LOKAL
	int i;
	bool win;
	
	// ALGORITMA
	i=0;
	win = false;
	while (i<121 && !win){
		if (isSama(P.akhirMenang[i], T)){
			win = true;
		} else{
			i++;
		}
	}
	return win;
}

bool isSeri(Tabel T, Papan P)
/*
	 Mengembalikan true jika Tabel T ada di kumpulan state seri pada Papan.
*/
{	
	// KAMUS LOKAL
	int i;
	bool draw;
	
	// ALGORITMA
	i=0;
	draw = false;
	while (i<14 && !draw){
		if (isSama(P.akhirSeri[i], T)){
			draw = true;
		} else{
			i++;
		}
	}
	return draw;
}

void mulai(Tabel *T, Papan P, int pilihan, State *S)
/* 
	 I.S. T sembarang
	 F.S. T akan diisi berdasarkan input dari pengguna dan isi tabel T akan diganti
	 			dari transition table yang ada di papan P yang bersal dari file eksternal.
*/
{
	// KAMUS LOKAL
	int i,isi;
	Tabel temp;
	bool found;
	
	// ALGORITMA
	
	// Cek pilihan untuk menentukan giliran main
	if (pilihan == 0){ // User main terlebih dahulu
		salinTabel(*T, &temp);
		tulisTabel(isiTabel(temp,4));
		printf("Giliran Komputer\n");
		*T = P.awal[2];
	}
	else{ // Komputer main terlebih dahulu
		*T = P.awal[1];
	}
	(*S).length = 0;
	(*S).state[(*S).length] = *T;
	tulisTabel(*T);
	salinTabel(*T, &temp);
	// Melakukan perulangan sampai keadaan tabel adalah komputer menang atau seri
	while(!isMenang(*T,P) && !isSeri(*T,P)){
		printf("\n");
		printf("Giliran Anda. Masukan input (1-9) = ");
		scanf("%d", &isi);
		while (isi>9 || isi<0){ // Melakukan perulangan jika input salah
			printf("Input salah!\n");
			printf("Giliran Anda. Masukan input (1-9) = ");
			scanf("%d", &isi);
		}
		i=0;
		found = false;
		// Akan mencari perbaris apakah terdapat matriks dengan kolom 0 yang nilainya sama dengan T.
		// Saat ditemukan, maka T akan berubah isinya dengan menggeser kolomnya sebanyak input/isi dengan baris yang tetap.
		while (i<=P.length && !found){
			if (isSama(P.state[i][0],*T)){
				*T = P.state[i][isi];
				found = true;
			} else{
				i++;
			}
		}
		// Jika ternyata saat T dirubah nilainya tetap sama (dalam konsep DFA dia kembali ke statenya sendiri)
		// Maka akan dilakukan perulanagan sampai didapat input yang valid, yaitu input yang tidak menuju ke irinya sendiri.
		while (isSama(*T, temp)){
			printf("############################################# TABEL SUDAH TERISI ##############################################\n");
			printf("\n");
			printf("Giliran Anda. Masukan input (1-9) = ");
			scanf("%d", &isi);
			while (isi>9 || isi<0){
				printf("Input salah!\n");
				printf("Giliran Anda. Masukan input (1-9) = ");
				scanf("%d", &isi);
			}
			i=0;
			found = false;
			while (i<=P.length && !found){
				if (isSama(P.state[i][0],*T)){
					*T = P.state[i][isi];
					found = true;
				} else{
					i++;
				}
			}
		}
		// Pengisian stae yang telah dilalui
		(*S).length++;
		(*S).state[(*S).length] = *T;
		temp = isiTabel(temp,isi-1);
		tulisTabel(temp);
		// Kalau belum menang, maka komputer main
		if (!isMenang(temp,P) && !isSeri(temp,P)){
			printf("Giliran Komputer\n");
			tulisTabel(*T);
			salinTabel(*T, &temp);
		}
	}
}

void tulisState(State S)
/* I.S. S sembarang
   F.S. Isi dari state S akan ditulis ke layar */
{
  // KAMUS LOKAL
  int i,j;
	
  //ALGORITMA
  printf("DAFTAR STATE YANG TELAH DILALUI\n");
  for (i=0; i<S.length+1; i++){
    for (j=0; j<9; j++){
      printf("%c", Elmt(S.state[i],j));
    }
    if (i!=(S.length)){
      printf(" ", S.state[i]);
    } else{
      printf("\n", S.state[i]);
    }
  }
}


int main(){
	// KAMUS
	Papan P;
	int loop, pilihan;
	Tabel T;
	State S;
	
	// ALGORITMA
	intro();
	bacaFile(&P);	
	
	loop = 0;
	while (loop != -999){
		inisialisasi(&T);
		printf("################################### KETIK 0 AGAR ANDA BERMAIN TERLEBIH DAHULU ###################################\n");
		printf("################################# KETIK 1 AGAR KOMPUTER BERMAIN TERLEBIH DAHULU #################################\n");
		printf("Masukkan Pilihan Anda = ");
		scanf("%d", &pilihan);
		while (pilihan!= 1 && pilihan!=0){
			printf("######################################## INPUT SALAH, SILAHKAN COBA LAGI ########################################\n");
			printf("################################### KETIK 0 AGAR ANDA BERMAIN TERLEBIH DAHULU ###################################\n");
			printf("################################# KETIK 1 AGAR KOMPUTER BERMAIN TERLEBIH DAHULU #################################\n");
			printf("Masukkan Pilihan Anda = ");
			scanf("%d", &pilihan);
		}
		// Memanggil algoritma mulai
		mulai(&T,P,pilihan,&S);
		
		// Pengecekan kondisi Tabel
		if (isMenang(T,P)){
			printf("KOMPUTER MENANG\n");
		} else{
			printf("DRAW\n");
		}
		
		tulisState(S);
		printf("\n");
		printf("KETIK -999 UNTUK KELUAR, KETIK APA SAJA UNTUK MAIN LAGI ");
		scanf("%d", &loop);
	}
	return 0;
}
