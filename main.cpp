#include <iostream>
#include <fstream>
#include <cstdlib>

#include <string>
#include <chrono>

using namespace std;

struct lista_s
{
    int v,w;
    lista_s* nastepny;
};

void wypisz_dane(int wierzcholki, int il_krawedz, fstream& strumien)
{
    int liczba_poz_wyk = il_krawedz - wierzcholki + 1;
    int tmp = 0;
    int numer_wierzcholka = wierzcholki;
    int licznik = 0;

    strumien.open("dane.txt", ios_base::app);

    strumien << wierzcholki<<" "<<il_krawedz<<" "<<rand()%wierzcholki<<endl;

    for(int i = 0; i < wierzcholki - 1 && i < il_krawedz; i++)


    {
        strumien << i<< " "<<i+1<<" "<<rand()%100+1<<endl;
    }


    while(licznik < liczba_poz_wyk)
    {
        tmp = 0;

        while(licznik < liczba_poz_wyk && tmp < numer_wierzcholka - 2)
        {
            strumien << numer_wierzcholka - 1<<" "<< tmp <<" "<<rand()%10+1<<endl;

            licznik++;
            tmp++;
        }
        numer_wierzcholka--;
    }
    strumien.close();

}

void generuj_dane(fstream& strumien)
{
    int wierzcholki[5] = {5, 10, 25, 50, 100};
    int max_krawedzi[5];
    int krawedzie[5][4];
    int wypelnienie[4] = {25, 50, 75, 100};

    strumien.open("dane.txt", ios_base::out);
    strumien.close();


    for (int i = 0; i < 5; i++)
    {
        max_krawedzi [i] = wierzcholki[i] * (wierzcholki[i] - 1) /2;
        for (int z = 0; z < 4; z++)
        {
            krawedzie[i][z] = wypelnienie[z] * max_krawedzi[i] /100;
            for(int c = 0; c < 100; c++) wypisz_dane (wierzcholki[i], krawedzie[i][z], strumien);
        }
    }

}

void wyswietl_sciezke(int* poprzednik, int* koszt_dojscia, int wierzch,fstream& strumien)
{
    int* wyswietlanie = new int [wierzch];
    int max_int =2147483647;
    int n = 0;
    int m;

    strumien.open("wyniki.txt", ios_base::app);

    for(int c = 0; c < wierzch; c++)
    {
        strumien << c <<": ";
        for(m = c; m != -1 && n < wierzch; m = poprzednik[m])wyswietlanie [n++] = m;

        while(n)strumien << wyswietlanie[--n] << " ";
        if (koszt_dojscia[c]==max_int){
            strumien << "$ brak polaczenia" << endl;
        }else {
            strumien << "$" << koszt_dojscia[c] << endl;
        }

    }
    strumien<<endl;
    delete [] wyswietlanie;
    strumien.close();
}

void BF_lista(int wierzch, int kraw, int start, lista_s** tab_list, fstream& wyjscie)
{
    lista_s* sasiedzi;

    int max_int =2147483647;

    int* koszt_dojscia;
    koszt_dojscia = new int [wierzch];

    int* poprzednik;
    poprzednik = new int [wierzch];

    for(int c = 0; c < wierzch; c++)
    {
        poprzednik[c] = -1;
        koszt_dojscia[c] = max_int;
    }

    koszt_dojscia[start] = 0;

    for(int c = 1; c < wierzch; c++)
    {
        for(int b = 0; b < wierzch; b++)
        {
            for(sasiedzi = tab_list[b]; sasiedzi; sasiedzi = sasiedzi -> nastepny)
            {
                if(koszt_dojscia[b] != max_int && koszt_dojscia[sasiedzi -> v] > koszt_dojscia[b] + sasiedzi -> w)
                {
                    koszt_dojscia[sasiedzi -> v] = koszt_dojscia [b] + sasiedzi -> w;
                    poprzednik[sasiedzi -> v] = b;
                }
            }
        }
    }
   wyswietl_sciezke(poprzednik, koszt_dojscia, wierzch,wyjscie);

}

void BF_macierz(int** graf_m, int wierzch, int kraw, int start, fstream& wyjscie)
{

    int max_int =2147483647;

    int* koszt_dojscia;
    koszt_dojscia = new int [wierzch];

    int* poprzednik;
    poprzednik = new int [wierzch];

    for(int c = 0; c < wierzch; c++)
    {
        poprzednik[c] = -1;
        koszt_dojscia[c] = max_int;
    }

    koszt_dojscia[start] = 0;

    for(int c = 1; c < wierzch; c++)
    {
        for(int b = 0; b < wierzch; b++)
        {
            for(int v = 0; v < wierzch; v++)
            {
                if(koszt_dojscia[b] != max_int && graf_m[v][b] != 0 && koszt_dojscia[v] > koszt_dojscia[b] + graf_m[v][b])
                {
                    koszt_dojscia[v] = koszt_dojscia [b] +  graf_m[v][b];
                    poprzednik[v] = b;
                }
            }
        }
    }
   wyswietl_sciezke(poprzednik, koszt_dojscia, wierzch, wyjscie);

}

void wyswietl_lista(lista_s** tab_list, int wierzch)
{

    lista_s* lista_w;
    cout<< "lista:\n";
    for(int i = 0; i < wierzch; i++)
    {
        cout << "tab[" << i << "] =";
        lista_w = tab_list[i];
        while(lista_w)
        {
            cout << lista_w->v <<" ";
            lista_w = lista_w->nastepny;
        }
        cout << endl;
    }
    delete []lista_w;
}

void graf_lista(int& wierzch, int& kraw, int& start, int** graf, lista_s** tab_list, lista_s* lista)
{
    int v1, v2, waga;

    for(int c = 0; c < wierzch; c++)
    {
        tab_list[c] = NULL;

    }

    for(int c = 0; c < kraw; c++)
    {
        v1 = graf[c][0];
        v2 = graf[c][1];
        waga = graf[c][2];

        lista = new lista_s;

        lista->v = v2;
        lista->w = waga;
        lista->nastepny = tab_list[v1];
        tab_list[v1] = lista;

    }

    for(int c = 0; c < kraw; c++)
    {
        v2 = graf[c][0];
        v1 = graf[c][1];
        waga = graf[c][2];

        lista = new lista_s;

        lista->v = v2;
        lista->w = waga;
        lista->nastepny = tab_list[v1];
        tab_list[v1] = lista;
    }

    /*wyswietl_lista(tab_list, wierzch);*/
}

void wyswietl_macierz(int** graf_m, int wierzch)
{


    for(int c = 0; c < wierzch; c++)
    {
        for(int v = 0; v < wierzch; v++)
        {
            cout << graf_m[c][v] <<" ";
        }
        cout<<endl;
    }
}

void** graf_macierz(int& wierzch, int& kraw, int& start, int** graf, int** graf_m)
{
    int v1, v2, waga;
    int p = 0;

    for (int v = 0; v < wierzch; ++v)
    {
        for(int b = p; b < wierzch; ++b)
        {
            graf_m[v][b] = 0;
            graf_m[b][v] = 0;
        }
        p++;
    }

    for (int v = 0; v < kraw; ++v)
    {
        v1 = graf[v][0];
        v2 = graf[v][1];
        waga = graf[v][2];
        graf_m[v1][v2] = waga;
        graf_m[v2][v1] = waga;
    }
    //wyswietl_macierz(graf_m, wierzch);
}

void** wczytaj_graf(int** graf, int& wierzch, int& kraw, int& start, ifstream& wejscie)
{
    //cout<< wierzch <<" "<<kraw<<" "<<start<<endl;
    for(int x = 0; x < kraw;x++)
    {
        wejscie >> graf[x][0];
        wejscie >> graf[x][1];
        wejscie >> graf[x][2];

       // cout<<graf[x][0]<<" ";
       // cout<<graf[x][1]<<" ";
       // cout<<graf[x][2]<<endl;
    }
}

double znajdz_d_lista(int** graf, int& wierzch, int& kraw, int& start)
{


    lista_s** tab_list;
    lista_s* lista, *usuwanie;
    fstream wyjscie;
    tab_list = new lista_s * [wierzch];

    graf_lista(wierzch, kraw, start, graf, tab_list, lista);

    auto time_start = std::chrono::high_resolution_clock::now();
    BF_lista(wierzch, kraw, start, tab_list, wyjscie);
    auto time_stop = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < wierzch; i++)
    {
        lista = tab_list[i];
        while(lista)
        {
            usuwanie = lista;
            lista = lista->nastepny;
            delete usuwanie;
        }
    }
    delete [] tab_list;

    return std::chrono::duration<double , std::milli>(time_stop-time_start).count();



    //wyjscie << "Czas trwania dla listy przy: "<<wierzch<<" i: "<<kraw*100/(wierzch * (wierzch - 1) /2)<<"% wypelnienia: "<<czas_trwania<<endl;




}

double znajdz_d_macierz(int** graf, int& wierzch, int& kraw, int& start)
{

    int** graf_m;
    fstream wyjscie;
    graf_m = new int *[wierzch];
    for(int v = 0; v < wierzch; ++v)
    {
        graf_m[v] = new int [wierzch];
    }

    graf_macierz(wierzch, kraw, start, graf, graf_m);
    auto time_start = std::chrono::high_resolution_clock::now();
    BF_macierz(graf_m, wierzch, kraw, start,wyjscie);
    auto time_stop = std::chrono::high_resolution_clock::now();

    for(int v = 0; v < wierzch; ++v)
    {
        delete [] graf_m[v];
    }
    delete [] graf_m;

    return std::chrono::duration<double , std::milli>(time_stop-time_start).count();




    //wyjscie << " dla macierzy: "<<czas_trwania<<endl;
}

void znajdz_droge(ifstream& wejscie)
{
    int j=0;
    double tczas1[20];
    double tczas2[20];
    int wierzch, kraw, start;
    int** graf;
    fstream pomiar;
    double czas1=0;
    double czas2=0;
    pomiar.open("pomiar.txt", ios_base::app);

    for(int i = 0; i < 2000; i++)
    {


        wejscie >> wierzch;
        wejscie >> kraw;
        wejscie >> start;

        graf = new int* [kraw];

        for(int c = 0; c < kraw; c++)
        {
            graf[c] = new int [3];
        }

        wczytaj_graf(graf, wierzch, kraw, start, wejscie);

        czas1 = czas1 + znajdz_d_macierz(graf, wierzch, kraw, start);

        czas2 = czas2 + znajdz_d_lista(graf, wierzch, kraw, start);


        if ((i+1)%100==0&&i!=0) {
            tczas1[j] = czas1/100;
            czas1 = 0;
            tczas2[j] = czas2/100;
            czas2 = 0;
            j++;
        }
        for(int z = 0; z < kraw; ++z)
        {
            delete [] graf[z];
        }
        delete [] graf;

    }

    for (j=0;j<20;j++){
        pomiar<<tczas1[j]<<";";
    }
    pomiar<<endl;
    for (j=0;j<20;j++){
        pomiar<<tczas2[j]<<";";
    }
    pomiar<<endl;
    pomiar.close();
}

void pomiar(ifstream& strumien)
{

    strumien.open("dane.txt", ios_base::in);
    znajdz_droge(strumien);
    strumien.close();

}



int main()
{
    int wybor = 3;

    fstream dane;
    ifstream wejscie;


    do
    {

        do
        {

            cout << "Wybierz opcje: 1. Wygeneruj dane do pliku  2. Pomiar \n";
            cin >> wybor;
            if(wybor > 2 || wybor < 1) cout << "Nie ma takiej opcji, wybierz ponownie\n";

        }while( wybor > 2 || wybor < 1 );

        switch( wybor )
        {
            case 1: generuj_dane(dane);
            case 2: pomiar(wejscie);
        }


    }while( wybor > 2 || wybor < 1 );


    return 0;
}