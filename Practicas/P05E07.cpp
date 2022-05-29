/* Una tecla posee un único registro de E/S : BTN_STATUS. 
Solo el bit menos significativo y el segundo bit menos significativo son de interés:

BTN_STATUS0: vale 0 si la tecla no fue pulsada, 1 si fue pulsada.
BTN_STATUS1: escribir 0 en este bit para limpiar la memoria de la tecla.

Escribir un driver para manejar este dispositivo de E/S. El driver debe retornar la constante
BTN_PRESSED cuando se presiona la tecla. Usar busy waiting. */

int driver_read(int *data){

    // espero a que presionen la tecla
    while(BTN_STATUS0 == 0){}
    // si el status es 1 entonces la tecla fue presionada
    if(BTN_STATUS0 == 1){
        leer_tecla();                   // hago lo q haya q hacer para leer la tecla  
        escribir_reg(BTN_STATUS1, 0);   // limpio la tecla para la prox escritura
        escribir_reg(BTN_STATUS0, 0);   // cambio el status a no presionada
    } 
    // si no es 0 y no es 1, retorno error
    else { return 1; }

    return 0;
}

int driver_write(int *data){

    //espero q haya pasado la lectura para escribir
    while(BTN_STATUS0 == 1){}
    //si es 0 entonces la tecla ya fue leida
    if(BTN_STATUS == 0){
        escribir_reg(BTN_STATUS0, 1);   // aviso que la tecla fue presionada
    }
    //si no es 0 ni 1, error
    else {return 1; }

    return BTN_PRESSED;
    
}