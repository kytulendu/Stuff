/* Skyrim Dragon font demo by Khralkatorrix */

#include <graphics.h>
#include <conio.h>

void main()
{
    int graphdriver = DETECT, graphmode, ErrorCode, DragFont, centertext;
    const char* FontName = "DRAG";
    const char* PATH_TO_BGI = "C:\\TC\\BGI\\";  /* change this to your BGI driver path */

    initgraph(&graphdriver, &graphmode, PATH_TO_BGI);
    ErrorCode = graphresult();  /* Read result of initialization */
    if (ErrorCode != grOk)      /* Error occured during init */
    {
        printf(" Graphics System Error: %s\n", grapherrormsg(ErrorCode));
        exit(1);
    }

    DragFont = installuserfont(FontName);
    if (graphresult() != grOk)
    {
        printf("Error installing %s", FontName);
        exit(1);
    }

    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(DragFont, HORIZ_DIR, 2);

    centertext = (getmaxx() / 2);

    outtextxy(centertext, 10, "DOV3K6N DOV3K6N");                   /* DOVahKiiN DOVahKiiN */
    outtextxy(centertext, 35, "N2L OK ZIN LOS V3R6N");              /* NaaL OK ZIN LOS VahRiiN */
    outtextxy(centertext, 60, "W3 D4N VOKUL M3FAER2K 3ST V2L");     /* Wah DeiN VOKUL MahFAERaaK ahST VaaL */
    outtextxy(centertext, 85, "3RK FIN NOROK P2L GR2N");            /* ahRK FIN NOROK PaaL GRaaN */
    outtextxy(centertext, 110, "FOD NUST HON ZINDRO Z2N");          /* FOD NUST HON ZINDRO ZaaN */
    outtextxy(centertext, 135, "DOV3K6N F3 HIN KOG2N MU DR2L");     /* DOVahKiiN Fah HIN KOGaaN MU DRaaL */

    outtextxy(centertext, 180, "3RK FIN KEL LOST PROD3");           /* ahRK FIN KEL LOST PRODah */
    outtextxy(centertext, 205, "DO VED V6NG KO FIN KR3");           /* DO VED ViiNG KO FIN KRah */
    outtextxy(centertext, 230, "TOL FOD Z5M3 WIN K4N M5Z FUND4N");  /* TOL FOD ZeyMah WIN KeiN MeyZ FUNDeiN */
    outtextxy(centertext, 255, "ALDUIN F5N DO JUN");                /* ALDUIN FeyN DO JUN */
    outtextxy(centertext, 280, "KRUZ6K VOKUN ST2DNAU");             /* KRUZiiK VOKUN STaaDNAU */
    outtextxy(centertext, 305, "VOTH 2N B3LOK W3 D6VON FIN L4N");   /* VOTH aaN BahLOK Wah DiiVON FIN LeiN */

    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(centertext, 350, "- Song of the Dragonborn");

    getch();
    closegraph();
}
