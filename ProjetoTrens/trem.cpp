#include "trem.h"
#include <QtCore>

// Inicializa os ponteiros estáticos
QMutex* Trem::regiao1 = nullptr;
QMutex* Trem::regiao2 = nullptr;
QMutex* Trem::regiao3 = nullptr;
QMutex* Trem::regiao4 = nullptr;
QMutex* Trem::regiao5 = nullptr;
QMutex* Trem::regiao6 = nullptr;
QMutex* Trem::regiao7 = nullptr;

//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->velocidade = 100;
}

//Velocidade
void Trem::setVelocidade(int velocidade){
    if(velocidade == 0){
        this->velocidade = 1000000;
    } else {
        this->velocidade = 200 - velocidade;
    }
}

// ======PIXELS MOVIDOS (COORDENADAS) ======
void Trem::right(){
    this->x += 10;
}

void Trem::left(){
    this->x -= 10;
}

void Trem::up(){
    this->y -= 10;
}

void Trem::down(){
    this->y += 10;
}
// ======PIXELS MOVIDOS (COORDENADAS) ======

void Trem::run(){
    while(true){
        switch(ID){
        case 1:
            if (y == 30 && x < 200){
                if (x == 180){
                    regiao1->lock();
                }
                right();
            }else if (x == 200 && y < 150){
                if (y == 130){
                    regiao3->lock();
                }
                down();
            }else if (x > 60 && y == 150){
                if (x == 180){
                    regiao1->unlock();
                }
                if (x == 110){
                    regiao3->unlock();
                }
                left();
            }else{
                up();
            }
            break;
            
        case 2:
            if (y == 30 && x < 340){
                if (x == 220) {
                    regiao1->unlock();
                }
                if (x == 320){
                    regiao2->lock();
                }
                right();
            }else if (x == 340 && y < 150){
                if (y == 130){
                    regiao5->lock();
                }
                down();
            }else if (x > 200 && y == 150){
                if (x == 320){
                    regiao2->unlock();
                }
                if (x == 290){
                    regiao4->lock();
                }
                if (x == 250){
                    regiao5->unlock();
                }
                if (x == 220){
                    regiao1->lock();
                }
                left();
            }else{
                if (x == 200 && y == 130){
                    regiao4->unlock();
                }
                up();
            }
            break;
            
        case 3:
            if (y == 30 && x < 480){
                if(x == 360){
                    regiao2->unlock();
                }
                right();
            }
            else if (x == 480 && y < 150)
                down();
            else if (x > 340 && y == 150){
                if(x == 430){
                    regiao6->lock();
                }
                if(x == 360){
                    regiao2->lock();
                }
                left();
            }
            else{
                if(x == 340 && y == 130){
                    regiao6->unlock();
                }
                up();
            }
            break;
            
        case 4:
            if (y == 150 && x < 270){
                if(x == 180){
                    regiao4->lock();
                }
                if(x == 250){
                    regiao7->lock();
                }
                if (x == 220){
                    regiao3->unlock();
                }
                right();
            }else if (x == 270 && y < 280){
                if (y == 170){
                    regiao4->unlock();
                }
                down();
            }else if (x > 130 && y == 280){
                if(x == 250){
                    regiao7->unlock();
                }
                left();
            }else{
                if(x == 130 && y == 170){
                    regiao3->lock();
                }
                up();
            }
            break;
            
        case 5:
            if (y == 150 && x < 410){
                if (x == 290){
                    regiao7->unlock();
                }
                if (x == 320){
                    regiao6->lock();
                }
                if (x == 360){
                    regiao5->unlock();
                }
                right();
            }else if (x == 410 && y < 280){
                if (y == 170){
                    regiao6->unlock();
                }
                down();
            }else if (x > 270 && y == 280){
                if (x == 290){
                    regiao7->lock();
                }
                left();
            }else{
                if (x == 270 && y == 170){
                    regiao5->lock();
                }
                up();
            }
            break;
            
        case 6:
            if (y == 30 && x > 60){
                if(x == 360){
                    regiao2->lock();
                }
                if(x == 340){
                    regiao2->unlock();
                }
                if(x == 220){
                    regiao1->lock();
                }
                if(x == 200){
                    regiao1->unlock();
                }
                left();
            }
            else if (x == 60 && y < 280){
                down();
            }
            else if (x < 480 && y == 280){
                if(x == 250){
                    regiao7->lock();
                }
                if(x == 270){
                    regiao7->unlock();
                }
                right();
            }
            else{
                up();
            }
            break;
            
        default:
            break;
        }
        
        emit updateGUI(ID, x, y); //Emite o sinal 
        msleep(velocidade);
    }
}