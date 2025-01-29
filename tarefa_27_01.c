#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "matriz_leds.h"

//definição das variáveis macros de escopo global
#define ledRed 13
#define btnA 5
#define btnB 6
uint a = 0;
static volatile lastTimeA = 0;
static volatile lastTimeB = 0;
PIO pio;
uint sm;

//criação das matrizes correspondentes a cada número a ser mostrado na matriz de leds
Matriz_leds_config um = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config zero = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config dois = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config tres = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config quatro = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config cinco = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config seis = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config sete = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config oito = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config nove = {
            //       Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
            //R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, {0.0, 0.0, 0.0}}  // Linha 4
            };
Matriz_leds_config* numeros[] = {&zero, &um, &dois, &tres, &quatro, &cinco, &seis, &sete, &oito, &nove};

//rotina de interrupção dos botões
void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t currentTime = to_us_since_boot(get_absolute_time());
    //tratamento debouncing
    if(gpio == btnA && currentTime - lastTimeA > 200000){
        lastTimeA = currentTime;
        a = (a + 1) % 10; //incrementa o contador
        printf("%d\n", a);
    //tratamento debouncing
    }else if(gpio == btnB && currentTime - lastTimeB > 200000){
        lastTimeB = currentTime;
        a = (a == 0) ? 9 : (a - 1); //decrementa o contador
        printf("%d\n", a);
    }
    imprimir_desenho(*numeros[a], pio, sm); //atualiza o número na matriz de leds
}
int main()
{
    //inicialização dos gpio's
    gpio_init(ledRed);
    gpio_set_dir(ledRed, GPIO_OUT);
    gpio_init(btnA);
    gpio_set_dir(btnA, GPIO_IN);
    gpio_pull_up(btnA);
    gpio_init(btnB);
    gpio_set_dir(btnB, GPIO_IN);
    gpio_pull_up(btnB);

    //inicialização da matriz de leds
    pio = pio0;
    sm = configurar_matriz(pio);
    imprimir_desenho(zero, pio, sm);

    //inicialização das rotinas de interrupção
    gpio_set_irq_enabled_with_callback(btnA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(btnB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    //blink do led vermelho de 5 vezes por segundo
    while (true) {
        for(int i = 0; i < 5; i++){
            gpio_put(ledRed, 1);  // LED aceso
            sleep_ms(100);        // Aguarda 100 ms
            gpio_put(ledRed, 0);  // LED apagado
            sleep_ms(100);        // Aguarda 100 ms
        }
    }
}
