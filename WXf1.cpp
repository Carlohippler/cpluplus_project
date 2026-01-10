#include <wx/wx.h>
#include <iostream>
#include <wx/msgdlg.h>
#include<wx/notebook.h>
#include <cstdlib>
#include <ctime>
#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include "sqlite3.h"

using namespace std;

//Banco de dados SQLite
void InitBancoDados() {

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("Config/dados.db", &db);

    if (rc) {

        wxMessageBox("Erro ao abrir banco de dados: " + wxString(sqlite3_errmsg(db)));
        return;
    }

    const char* sqlHistorico = "CREATE TABLE IF NOT EXISTS HISTORICO("\
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
        "NOME TEXT NOT NULL,"\
        "MEDIA REAL NOT NULL,"\
        "STATUS TEXT NOT NULL);";

    const char* sqlConfig = "CREATE TABLE IF NOT EXISTS CONFIGS("\
        "CHAVE TEXT PRIMARY KEY,"\
        "VALOR TEXT NOT NULL);";

    sqlite3_exec(db, sqlHistorico, NULL, 0, &zErrMsg);
    sqlite3_exec(db, sqlConfig, NULL, 0, &zErrMsg);

    sqlite3_close(db);


}


// a classe da Janela (Frame)
class MyFrame : public wxFrame {
public:

    //Daqui pra baixo é tudo UI
    MyFrame() : wxFrame(NULL, wxID_ANY, "Media Escola", wxDefaultPosition, wxSize(850, 600)) {


        wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxPoint(0, 0), wxSize(840, 600), wxNB_RIGHT);
        

        wxPanel* panel = new wxPanel(notebook, wxID_ANY);
        wxPanel* historico = new wxPanel(notebook, wxID_ANY);      
        wxPanel* config = new wxPanel(notebook, wxID_ANY);
        
        
        notebook->AddPage(panel, "Calculo");
        notebook->AddPage(historico, "Hist");   
        notebook->AddPage(config, "Config");
        

        panel->SetBackgroundColour(*wxWHITE);
        historico->SetBackgroundColour(*wxWHITE);
        config->SetBackgroundColour(*wxWHITE);



        wxPanel* menulateral = new wxPanel(this, wxID_ANY, wxPoint(600, 0), wxSize(250, 600));
        menulateral->SetBackgroundColour(wxColour(200, 200, 200));
        menulateral->Hide();

        wxPanel* login = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(850, 600));
        login->Raise();
        login->Hide();



        wxImage imglogo;
        if (imglogo.LoadFile("Config/logo_inicialC.png", wxBITMAP_TYPE_ANY)) {

            imglogo.Rescale(100, 100, wxIMAGE_QUALITY_HIGH);

            wxStaticBitmap* logotipo = new wxStaticBitmap(panel, wxID_ANY,wxBitmap(imglogo ), wxPoint(10,0));
        }


        wxStaticText* texto = new wxStaticText(panel, wxID_ANY, "Digite o Nome do Aluno:  ", wxPoint(150, 20));
        this->Center();

        wxTextCtrl* campoNome = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(140, 50), wxSize(200, -1));

        wxString nomealuno;

        wxButton* menuAbas = new wxButton(this, wxID_ANY, "Menu", wxPoint(740, 10), wxSize(40, 20));
        menuAbas->Raise();

        wxButton* CriarMedia = new wxButton(panel, wxID_ANY, "Criar Media", wxPoint(700, 50));

        wxButton* Clear = new wxButton(panel, wxID_ANY, "Limpar", wxPoint(705, 100));

        wxButton* ClearOnly = new wxButton(panel, wxID_ANY, "Limp Notas", wxPoint(700, 150));

        wxPanel* fundoBarra = new wxPanel(panel, wxID_ANY, wxPoint(100, 400), wxSize(350, 20));
        fundoBarra->SetBackgroundColour(wxColour(50, 50, 50));

        wxPanel* preenchimento = new wxPanel(fundoBarra, wxID_ANY, wxPoint(0, 0), wxSize(0, 20));
        preenchimento->SetBackgroundColour(*wxGREEN);

        wxStaticText* configuracao = new wxStaticText(config, wxID_ANY, "Configurações", wxPoint(350, 20));

        wxStaticText* textonota1 = new wxStaticText(panel, wxID_ANY, "Digite a primeira nota do Aluno:", wxPoint(55, 100));

        wxStaticText* textonota2 = new wxStaticText(panel, wxID_ANY, "Digite a nota de recuperação:", wxPoint(270, 100));

        wxStaticText* textonota3 = new wxStaticText(panel, wxID_ANY, "Digite a segunda nota do Aluno:", wxPoint(55, 200));

        wxStaticText* textonota4 = new wxStaticText(panel, wxID_ANY, "Digite a nota de recuperação:", wxPoint(270, 200));

        wxStaticText* textoMedia = new wxStaticText(panel, wxID_ANY, "Resultado da Media do Aluno:", wxPoint(125, 280));

        wxStaticText* resolu = new wxStaticText(panel, wxID_ANY, "", wxPoint(125, 300));

        wxTextCtrl* campoNota1 = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(55, 125), wxSize(170, -1));

        wxTextCtrl* campoNota2 = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(270, 125), wxSize(170, -1));

        wxTextCtrl* campoNota3 = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(55, 225), wxSize(170, -1));

        wxTextCtrl* campoNota4 = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(270, 225), wxSize(170, -1));

        wxListBox* lsHistorico = new wxListBox(historico, wxID_ANY, wxPoint(20, 20), wxSize(570, 350));

        wxButton* limpHist = new wxButton(historico, wxID_ANY, "Limpar Historico",  wxPoint(20, 380));

        wxButton* mudarBg = new wxButton(config, wxID_ANY, "Mudar Cor", wxPoint(100, 200));

        wxButton* voltarbranco = new wxButton(config, wxID_ANY, "Padrão", wxPoint(100, 250));

        wxButton* Calculadora = new wxButton(menulateral, wxID_ANY, "Calculo", wxPoint(125, 25));

        wxButton* historicoL = new wxButton(menulateral, wxID_ANY, "Historico", wxPoint(125, 75));
        
        wxButton* configL = new wxButton(menulateral, wxID_ANY, "Configurações", wxPoint(105, 450));


        Calculadora->Bind(wxEVT_BUTTON, [notebook, menulateral, menuAbas](wxCommandEvent&) {

            notebook->SetSelection(0);
           

            wxCommandEvent evento(wxEVT_BUTTON, menuAbas->GetId());
            menuAbas->GetEventHandler()->ProcessEvent(evento);

            });

        historicoL->Bind(wxEVT_BUTTON, [notebook, menulateral, menuAbas](wxCommandEvent&) {

            notebook->SetSelection(1);
            
            wxCommandEvent evento(wxEVT_BUTTON, menuAbas ->GetId());
            menuAbas->GetEventHandler()->ProcessEvent(evento);

            });

        configL->Bind(wxEVT_BUTTON, [notebook, menulateral, menuAbas](wxCommandEvent&) {
          
            notebook->SetSelection(2);
            
            wxCommandEvent evento(wxEVT_BUTTON, menuAbas->GetId());
            menuAbas->GetEventHandler()->ProcessEvent(evento);

            });

        menuAbas->Bind(wxEVT_BUTTON, [panel, historico, this, menulateral, voltarbranco, mudarBg, Clear, ClearOnly, CriarMedia, notebook, menuAbas] (wxCommandEvent&) {
        
            if (menulateral->IsShown()) {
                menulateral->Hide();

                menuAbas ->SetPosition(wxPoint(740, 10));
                CriarMedia->SetPosition(wxPoint(700, 50));
                Clear->SetPosition(wxPoint(705, 100));
                ClearOnly->SetPosition(wxPoint(700, 150));

                this->Refresh();

            }
            else {
                menulateral->Show();
                menulateral->Raise();

                menuAbas->SetPosition(wxPoint(540, 10));
                CriarMedia->SetPosition(wxPoint(500, 50));
                Clear->SetPosition(wxPoint(505, 100));
                ClearOnly->SetPosition(wxPoint(500, 150));

                this->Refresh();
            }

            
              
        

            });

        voltarbranco->Bind(wxEVT_BUTTON, [panel, historico, texto, textonota1, textonota2, textonota3, textonota4, preenchimento](wxCommandEvent&) {

            panel->SetBackgroundColour(*wxWHITE);
            historico->SetBackgroundColour(*wxWHITE);
            preenchimento->SetBackgroundColour(*wxGREEN);

            texto->SetForegroundColour(*wxBLACK);
            textonota1->SetForegroundColour(*wxBLACK);
            textonota2->SetForegroundColour(*wxBLACK);
            textonota3->SetForegroundColour(*wxBLACK);
            textonota4->SetForegroundColour(*wxBLACK);

            panel->Refresh();
            historico->Refresh();
            });
    
        

        
            mudarBg->Bind(wxEVT_BUTTON, [panel, historico, textonota1, textonota2, textonota3, textonota4, texto, preenchimento, resolu](wxCommandEvent&) {

                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;

                wxColour corAleatoria(r, g, b);

                panel->SetBackgroundColour(corAleatoria);
                historico->SetBackgroundColour(corAleatoria);
                preenchimento->SetBackgroundColour(wxColour(256 -r, 256 -g, 256 -b));
                
                panel->Refresh();
                

                if ((r + g + b) / 3 < 128) {

                    texto->SetForegroundColour(*wxWHITE);
                    textonota1->SetForegroundColour(*wxWHITE);
                    textonota2->SetForegroundColour(*wxWHITE);
                    textonota3->SetForegroundColour(*wxWHITE);
                    textonota4->SetForegroundColour(*wxWHITE);
                    resolu->SetForegroundColour(wxColour(84, 240, 84));
                    

                }
                else {

                    texto->SetForegroundColour(*wxBLACK);
                    textonota1->SetForegroundColour(*wxBLACK);
                    textonota2->SetForegroundColour(*wxBLACK);
                    textonota3->SetForegroundColour(*wxBLACK);
                    textonota4->SetForegroundColour(*wxBLACK);
                    resolu ->SetForegroundColour(wxColour(8, 48, 8));
                    
                }

                panel->Refresh();
                historico->Refresh();

                });
        
    
        
        limpHist->Bind(wxEVT_BUTTON, [lsHistorico](wxCommandEvent&) {
            lsHistorico->Clear();
            });

        ClearOnly->Bind(wxEVT_BUTTON, [campoNota1, campoNota2, campoNota3, campoNota4, resolu, preenchimento](wxCommandEvent&) {
          
            campoNota1->Clear();
            campoNota2->Clear();
            campoNota3->Clear();
            campoNota4->Clear();
            preenchimento->SetSize(0, 20);

            resolu->SetLabel("Media: ---");
            resolu->SetForegroundColour(*wxBLACK);

            });

        Clear->Bind(wxEVT_BUTTON, [campoNome, campoNota1, campoNota2, campoNota3, campoNota4, resolu, preenchimento](wxCommandEvent&){

            campoNome->Clear();
            campoNota1->Clear();
            campoNota2->Clear();
            campoNota3->Clear();
            campoNota4->Clear();
            preenchimento->SetSize(0, 20);
            
            resolu->SetLabel("Media: ---");

            });


        CriarMedia->Bind(wxEVT_BUTTON, [campoNome, resolu, campoNota1, campoNota2, campoNota3, campoNota4, lsHistorico, preenchimento](wxCommandEvent&) {

            wxString nomealuno = campoNome->GetValue();

            double res1, res2;
            double res;

            //Aqui é funçao de campo de identificaçao de erro de escrita num

            double nota1, nota2, nota3, nota4;

            if (!campoNota1->GetValue().ToDouble(&nota1) ||
                !campoNota2->GetValue().ToDouble(&nota2) ||
                !campoNota3->GetValue().ToDouble(&nota3) ||
                !campoNota4->GetValue().ToDouble(&nota4)) {
                wxMessageBox("Erro: Digite um Valor numerico valido!");
                return;
            }

            if (nota1 < 0 || nota1 >100 || nota2 < 0 || nota2 >100 ||
                nota3 < 0 || nota3 >100 || nota4 < 0 || nota4 >100) {
                wxMessageBox("Erro: Digite uma nota valida entre 0 e 100!");

                return;
            }

            if (nota1 >= nota2) {
                res1 = nota1;
            }
            else {
                res1 = nota2;
            }

            if (nota3 >= nota4) {
                res2 = nota3;
            }
            else {
                res2 = nota4;
            }

            res = (res1 + res2) / 2;

            wxString status = (res >= 60) ? "Aprovado" : "Reprovado";

            wxString strRes = wxString::Format("Media Final: %.1f", res);
            resolu->SetLabel(strRes);

            if (res >= 60) {
                resolu->SetForegroundColour(*wxGREEN);
            }
            else {
                resolu->SetForegroundColour(*wxRED);
            }

            resolu->Refresh();

            if (!nomealuno.IsEmpty()) {

                //Barra de progressão
                for (int i = 0; i <= 100; i += 5) {

                    int larguraNova = (i * 350) / 100;
                    preenchimento->SetSize(larguraNova, 20);

                    preenchimento->Update();

                    wxMilliSleep(50);
                }

                sqlite3* db;
                if (sqlite3_open("Config/dados.db", &db) == SQLITE_OK) {

                    wxString query = wxString::Format("INSERT INTO HISTORICO (NOME, MEDIA, STATUS) VALUES ('%s', %.1f, '%s');",
                        nomealuno, res, status);

                    char* zErrMsg = 0;

                    int rc = sqlite3_exec(db, query.mb_str(), NULL, 0, &zErrMsg);

                    if (rc != SQLITE_OK) {

                        wxMessageBox("Erro SQL: " + wxString(zErrMsg));
                        sqlite3_free(zErrMsg);

                    }
                    sqlite3_close(db);

            }



            wxString msgFinal = wxString::Format("A média do aluno %s é %.1f. Status: %s", nomealuno, res, status);

            wxMessageBox(msgFinal);

            wxString itemHistorico = nomealuno + " - Média: " + wxString::Format("%.1f (%s)", res, status);
            lsHistorico->Append(itemHistorico);
            
            preenchimento->SetSize(0, 20);

        }
        else {
            wxMessageBox("Por favor digite o nome do Aluno antes de Posseguir!");
        }


    });

   
        this->Centre(); 

    } 

    

}; 

//a classe da Aplicação
class MyApp : public wxApp {
public:

    virtual bool OnInit() {
        srand(time(NULL));
        wxInitAllImageHandlers();
        

        wxString config = "Config";

        if (!wxDirExists(config)) {
            wxMkdir(config);
        }

        InitBancoDados();
       
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        

        // Mensagem de boas-vindas depois da janela carregar
        // wxMessageBox("Bem Vindo ao sistema de media escolar!");

        return true;

    }
};

// Inicia a aplicação
wxIMPLEMENT_APP(MyApp);