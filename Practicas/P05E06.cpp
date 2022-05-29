/* Un cronómetro posee 2 registros de E/S:
CHRONO_CURRENT_TIME que permite leer el tiempo medido,
CHRONO_CTRL que permite ordenar al dispositivo que reinicie el contador.
El cronómetro reinicia su contador escribiendo la constante CHRONO_RESET en el registro de control.
Escribir un driver para manejar este cronómetro. Este driver debe devolver el tiempo actual cuando
invoca la operación read(). Si el usuario invoca la operación write(), el cronómetro debe reiniciarse. */


int driver_read(int *data){
    int time = CHRONO_CURRENT_TIME;
    return time;
} 

int driver_write(int *data){
    //NO ESPECIFICA USO, DOS OPCIONES?
    //OP1
    CHRONO_CTRL = CHRONO_RESET;
    return 0;
    //OP2
    WRITE_CHRONO_CTRL(CHRONO_RESET);
    return 0;
}
