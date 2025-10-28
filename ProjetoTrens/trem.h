#ifndef TREM_H
#define TREM_H

#include <QThread>
#include <QMutex>

/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função TERMINATE.
 *
*/
class Trem: public QThread{
 Q_OBJECT
public:
    Trem(int,int,int);
    void run();
    void setVelocidade(int);
    
    // Mutexes estáticos das 7 regiões críticas
    static QMutex *regiao1;
    static QMutex *regiao2;
    static QMutex *regiao3;
    static QMutex *regiao4;
    static QMutex *regiao5;
    static QMutex *regiao6;
    static QMutex *regiao7;

signals:
    void updateGUI(int,int,int);

private:
   int x;           //posição X do trem na tela
   int y;           //posição Y do trem na tela
   int ID;          //ID do trem
   int velocidade;  //Velocidade. É o tempo de dormir em milisegundos entre a mudança de posição do trem
   
   void right();
   void left();
   void up();
   void down();
};

#endif // TREM_H
