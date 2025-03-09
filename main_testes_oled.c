#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"
#include "ssd1306/ssd1306_bitmaps.h"

const char* longText = "Este eh um exemplo de texto muito longo que nao cabe na tela e precisa rolar horizontalmente. Fontes em codigo ASCII (Renderizar caracteres Unicode pode ser mais lento do que caracteres ASCII, especialmente se a fonte for grande. Por isso optei por utilizar ASCII) #$%'''===@??&/$()!*+-//|<>;,._0123456789. VIRTUS-CC. Estou implementando a biblioteca para este display oled SSD1306. Para a placa Raspberry pi pico W.";

int main() {

   stdio_init_all();           // Inicializa todos os tipos stdio padrão presentes que estão vinculados ao binário.
   ssd1306_Init();             // Inicializar a exibição do display
   ssd1306_Fill(Black);        // Preenche a tela com a cor de fundo preto

                     //=== Exemplos comentados (remover/incluir comentários de acordo com o teste) ===

//=== Desenha o bitmap na tela com a cor branca e tamanho definido para o ssd1306 ===

   ssd1306_DrawBitmap(0, 0, virtuscc_bitmap, SSD1306_WIDTH, SSD1306_HEIGHT, White);

   //ssd1306_DrawBitmap(0, 0, astronauta, SSD1306_WIDTH, SSD1306_HEIGHT, White);

   //ssd1306_DrawBitmap(0, 0, arrow_bitmap, 8, 8, White);

//=== Desenha um retângulo de (0, 0) a (127, 31) com a cor branca. Exemplo de uso como moldura, e etc. ===
   //ssd1306_DrawRectangle(0, 0, 127, 63, White);
   //ssd1306_FillRectangle(0, 0, 127, 63, White);  // retângulo preenchido        

//=== Desenha um triângulo com a cor branca. Exemplo de uso junto da moldura e/ou com outras formas geométricas, etc. (é sobreposto) ===
   //ssd1306_DrawTriangle(10, 10, 50, 10, 30, 40, White);
   //ssd1306_FillTriangle(10, 10, 50, 10, 30, 40, White); // triângulo preenchido
 
//=== Desenha um circulo com a cor branca. Exemplo de uso junto da moldura e/ou com outras formas geométricas, etc. (é sobreposto) ===   
   //ssd1306_DrawCircle(SSD1306_WIDTH - 20, SSD1306_HEIGHT - 20, 10, White);
   //ssd1306_FillCircle(SSD1306_WIDTH - 20, SSD1306_HEIGHT - 20, 10, White); // circulo preenchido

//=== O texto será exibido com quebra automatica de linha ou/e com "\n": === 

   /* 
   ssd1306_SetCursor(1, 1);                              // Coloca o cursor na posição
   ssd1306_WriteStringWrapped("Um exemplo de texto com quebra automatica!\nE nova linha automaticamente!", Font_7x10, White);   
   */

//=== Se ultrapassar o tamanho do display o texto não será exibido se não pular a linha com "\n" manualmente: === 
   
   /* 
   ssd1306_SetCursor(1, 1);                              // Coloca o cursor na posição
   ssd1306_WriteString("Um exemplo de texto sem quebra automatica que será cortado!", Font_7x10, White);  
   */
 
//=== Inicia scroll horizontalmente da tela, automáticamente, da página 0 até 7 com velocidade definida (0 a 7, onde 0 é mais rápido(terceiro número)) === 
   //ssd1306_StartScrollLeft(0, 7, 0); // scroll da esquerda
   //ssd1306_StartScrollRight(0, 7, 0); // scroll da direita

    
//=== Atualizar a exibição (manter ativo) ===
   ssd1306_UpdateScreen();          

   while (1)
   {
//=== Exibe em loop um texto que não cabe totalmente na tela fazendo o scroll horizontal (sem quebra de linha). === 

    //ssd1306_ScrollTextHorizontal(longText, Font_7x10, White, 10, 1); 
   }    
   
   return 0;
}
