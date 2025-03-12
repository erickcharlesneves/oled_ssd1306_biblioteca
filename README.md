# Biblioteca para o SSD1306 - Display OLED para BitDogLab / Raspberry Pi Pico W 


A presente biblioteca tem como objetivo auxiliar em projetos de sistemas embarcados que utilizem o display OLED SSD1306 com a plataforma de desenvolvimento BitDogLab/Raspberry Pi Pico W, baseada no microcontrolador RP2040. 

O código da biblioteca foi desenvolvido na linguagem **C**, também com auxilio da extensão ***CMake***, e o ambiente de desenvolvimento escolhido para a implementação foi o ***Visual Studio Code***. Juntamente com a extensão **_Raspberry Pi Pico_**, disponível na ferramenta de desenvolvimento. 

Esta biblioteca abstrai os comandos de baixo nível e oferece um conjunto robusto de funções para inicialização, desenho gráfico, escrita de texto e até efeitos como scroll, etc.

Este display OLED usa o protocolo I2C ou SPI para se comunicar, sendo necessários respectivamente 2 ou 5 pinos para a interface.

Utilizarei nesta biblioteca o I2C sendo assim 2 pinos para interfaceamento.
Conforme as duas linhas de comunicação:
    
>   - SDA (Serial Data Line): Linha de dados, onde os bits são transmitidos, conectado ao **GPIO14**.
>         
>  - SCL (Serial Clock Line): Linha de clock, que sincroniza a comunicação, conectado ao **GPIO15**.

Conforme está definido no arquivo ssd1306.c da pasta da biblioteca :
```c
if  defined(SSD1306_USE_I2C) //Verifica se o protocolo I2C está habilitado.

//Define os pinos SDA (dados) como GPIO14 e SCL (clock) como GPIO15.

const  uint8_t  I2C_SDA_PIN  =  14;

const  uint8_t  I2C_SCL_PIN  =  15;
```
----------
## Esta biblioteca fornece uma interface fácil de usar com diversas camadas e funcionalidades:

### 1. Configuração e Setup

**Arquivo: ssd1306_conf.h**

-   **Seleção do barramento:**  
    Define se a comunicação será via I2C (ou SPI, se habilitado). No exemplo da biblioteca como dito anteriormente, está configurado para I2C.
-   **Configurações de hardware:**  
    Define o pino de I2C, o endereço do display (0x3C), e configura as dimensões do display (largura e altura).
-   **Ajuste de Fontes:**  
    Permite incluir apenas as fontes necessárias, economizando memória.

Essa camada de configuração torna a biblioteca altamente portável e personalizável para diferentes plataformas e resoluções.

----------

### 2. Comunicação de Baixo Nível

**Funções principais:**

-   **`ssd1306_WriteCommand(uint8_t byte):`**  
    Prepara um buffer com um byte de controle (0x00) seguido pelo comando a ser enviado e utiliza a função de escrita bloqueante do I2C para transmitir o comando ao display.
    
-   **`ssd1306_WriteData(uint8_t_ buffer, size_t buff_size):`**  
    Similar à função de comando, porém prepara um buffer com o byte de controle 0x40, indicando que os dados a seguir serão gravados na memória de vídeo.
    

Essas funções encapsulam a comunicação com o display, permitindo que as funções de nível superior se concentrem na lógica de desenho e controle.

----------

### 3. Inicialização e Atualização do Display

**Função: `ssd1306_Init()`**

-   **Inicialização do I2C e configuração dos pinos:**  
    Configura os pinos SDA e SCL, ativa os resistores de pull-up e inicializa o barramento I2C.
-   **Sequência de comandos para configurar o display:**  
    Inclui comandos para desativar o scroll (garantindo que estados anteriores não interfiram), definir o modo de endereçamento, ajustar o mapeamento dos segmentos (espelhamento vertical/horizontal, se necessário), configurar o contraste, definir a multiplexação, a frequência do clock e outras configurações importantes conforme o datasheet do SSD1306.
-   **Limpeza e atualização:**  
    Após configurar os registros internos, a função limpa a tela (preenche o buffer com preto ou branco) e envia o conteúdo do buffer para o display com `ssd1306_UpdateScreen()` .

Essa função garante que o display esteja em um estado conhecido e configurado corretamente antes de iniciar qualquer operação gráfica.

----------

### 4. Manipulação do Buffer de Vídeo

-   **Buffer Interno (SSD1306_Buffer):**  
    É um array estático que representa o estado de cada pixel do display (1 bit por pixel). Todas as operações de desenho são realizadas neste buffer antes de serem enviadas para o display físico.
    
-   **`ssd1306_Fill(SSD1306_COLOR color):`**  
    Preenche todo o buffer com a cor desejada (preto para 0x00 ou branco para 0xFF), permitindo limpar a tela de forma rápida.
    
-   **`ssd1306_FillBuffer(uint8_t_ buf, uint32_t len):`**  
    Permite copiar um buffer pré-definido para o buffer interno, útil para atualizações rápidas ou para restaurar um estado salvo.
    
-   **`ssd1306_UpdateScreen():`**  
    Divide a tela em “páginas” (blocos de 8 pixels de altura) e envia cada parte do buffer para o display via I2C. Essa função “sincroniza” o buffer de memória com o que é efetivamente exibido.
    

----------

### 5. Funções Gráficas

A biblioteca oferece diversas funções para desenho de formas geométricas:

-   **`ssd1306_DrawPixel(x, y, color):`**  
    Desenha um pixel na posição especificada do buffer. Ela verifica os limites para evitar escrita fora da área válida.
    
-   **`ssd1306_Line(x1, y1, x2, y2, color):`**  
    Implementa o algoritmo de Bresenham para desenhar linhas entre dois pontos, garantindo linhas contínuas e precisas.
    
-   **`ssd1306_Polyline(vertex, size, color):`**  
    Desenha uma série de linhas conectando uma sequência de vértices, útil para desenhar formas poligonais abertas.
    
-   **`ssd1306_DrawRectangle()` e `ssd1306_FillRectangle()`:**  
    Permitem desenhar o contorno de um retângulo ou preencher um retângulo, iterando sobre as coordenadas de forma controlada.

-   **`ssd1306_DrawTriangle()` e `ssd1306_FillTriangle()`:**  
    Permitem desenhar o contorno de um triângulo ou preencher um triângulo, iterando sobre as coordenadas de forma controlada.
    
-   **`ssd1306_DrawCircle()` e `ssd1306_FillCircle():`**  
    Utilizam o algoritmo de Bresenham para desenhar círculos (contorno e preenchido, respectivamente).
    
-   **`ssd1306_DrawArc()` e `ssd1306_DrawArcWithRadiusLine()`:**  
    Permitem desenhar segmentos circulares (arcos), com a opção de adicionar linhas de raio para destacar o início e o fim do arco.
    
-   **`ssd1306_InvertRectangle()`:**  
    Inverte as cores dos pixels em uma região retangular, útil para criar efeitos de destaque ou “botões” interativos.
    
-   **`ssd1306_DrawBitmap()`:**  
    Permite desenhar imagens bitmap no display, interpretando os dados do bitmap e mapeando-os para os pixels do buffer.
    

#### Essas funções formam a base para qualquer aplicação gráfica, permitindo desde a criação de gráficos simples até interfaces complexas.

----------

### 6. Funções de Texto

A biblioteca também facilita a exibição de texto utilizando fontes pré-definidas:

-   **`ssd1306_WriteChar(ch, Font, color)`:**  
    Desenha um caractere utilizando uma fonte definida. Ele consulta os dados da fonte (bitmap do caractere) e desenha cada pixel de acordo com o valor correspondente. Suporta fontes monoespaçadas ou proporcionais (quando `Font.char_width` é definido).
    
-   **`ssd1306_WriteString(str, Font, color)`:**  
    Escreve uma string completa, chamando `ssd1306_WriteChar()` para cada caractere e avançando o cursor horizontalmente.
    
-   **`ssd1306_SetCursor(x, y)`:**  
    Define a posição inicial (em pixels) onde o próximo caractere será desenhado. Isso permite posicionar o texto em qualquer parte do display.
    
-   **`ssd1306_WriteStringWrapped(str, Font, color)`:**  
    É uma função para escrever texto com quebra automática de linha. Se o texto ultrapassar a largura do display, o cursor é reposicionado para a próxima linha. Além disso, interpreta o caractere de nova linha (`\n`).
    

#### Essas funções simplificam a exibição de mensagens, menus ou qualquer informação textual sem que precisemos lidar com a complexidade de desenhar cada caractere manualmente.

----------

### 7. Funções de Scroll (Rolagem)

A biblioteca incorpora funções para utilizar o recurso de scroll do próprio hardware do SSD1306:

-   **`ssd1306_StartScrollRight()` e `ssd1306_StartScrollLeft()`:**  
    Enviam uma sequência específica de comandos para iniciar a rolagem horizontal (para a direita ou para a esquerda) entre páginas definidas. Os parâmetros permitem configurar a página inicial, a página final e a velocidade (definida por um intervalo).
    
-   **`ssd1306_StopScroll()`:**  
    Envia o comando para interromper o scroll, retornando o display ao estado estático.
    

Essas funções são úteis para criar efeitos de rolagem de tela ou para exibir textos que não cabem de uma só vez, sem a necessidade de implementar o scroll manualmente via software.

Além disso, a biblioteca inclui uma função extra para **scroll de texto longo horizontal:** **`ssd1306_ScrollTextHorizontal`**, que redesenha uma linha de texto com deslocamento gradual – permitindo exibir a continuidade de um texto longo sem quebra de linha ou maior que o tamanho limite que caberia no display.

----------

### 8. Controle do Display

Outras funções importantes para o controle do display incluem:

-   **`ssd1306_SetContrast(value)`:**  
    Ajusta o contraste do display, alterando a intensidade dos pixels.
    
-   **`ssd1306_SetDisplayOn(on)` e `ssd1306_GetDisplayOn()`:**  
    Permitem ligar ou desligar o display, bem como consultar o estado atual (ligado ou desligado).
    

Essas funções facilitam a implementação de modos de economia de energia ou de diferentes níveis de brilho conforme a aplicação.

----------
## Exemplos Práticos:

No arquivo *main_testes_oled.c* podemos testar alguns exemplos práticos, para facilitar e agilizar note que a maioria das funções estarão comentadas com `//` ou `/* bloco a descomentar */` para testar individualmente preferencialmente ou se desejar em conjunto (se possível de acordo com os limites do display).

 **1. Bitmaps:**

```c
//=== Desenha o bitmap na tela com a cor branca e tamanho definido para o ssd1306 ===

ssd1306_DrawBitmap(0, 0, virtuscc_bitmap, SSD1306_WIDTH, SSD1306_HEIGHT, White); 
```
Bitmap de exemplo com a imagem do logo Virtus CC:

e (antes da execução da próxima função lembre de comentar `//` novamente a função acima).
```c
//ssd1306_DrawBitmap(0, 0, astronauta, SSD1306_WIDTH, SSD1306_HEIGHT, White);
```
Bitmap de exemplo com a imagem do astronauta:
  

Imagens utilizadas redimensionadas também na pasta: *img bitmap sample*

O bitmap foi gerado a partir do site:
https://diyusthad.com/image-to-cpp-v2

Com, Output Format Adafruit GFX Font: 
Que estão na pasta da biblioteca modularizada para os bitmaps *ssd1306_bitmaps.c*

 **2. **Algumas formas geométricas:****
Essas funções formam a base para qualquer aplicação gráfica, permitindo desde a criação de gráficos simples até interfaces complexas.
```c
//=== Desenha um retângulo de (0, 0) a (127, 31) com a cor branca. Exemplo de uso como moldura, e etc. ===

ssd1306_DrawRectangle(0, 0, 127, 63, White);

//ssd1306_FillRectangle(0, 0, 127, 63, White); // retângulo preenchido

  
//=== Desenha um triângulo com a cor branca. Exemplo de uso junto da moldura e/ou com outras formas geométricas, etc. (é sobreposto) ===

//ssd1306_DrawTriangle(10, 10, 50, 10, 30, 40, White);

ssd1306_FillTriangle(10, 10, 50, 10, 30, 40, White); // triângulo preenchido


//=== Desenha um circulo com a cor branca. Exemplo de uso junto da moldura e/ou com outras formas geométricas, etc. (é sobreposto) ===

ssd1306_DrawCircle(SSD1306_WIDTH - 20, SSD1306_HEIGHT - 20, 10, White);

//ssd1306_FillCircle(SSD1306_WIDTH - 20, SSD1306_HEIGHT - 20, 10, White); // circulo preenchido
```
Inserir imagem


 **3. Funções de Texto:**

```c
//=== O texto será exibido com quebra automatica de linha ou/e com "\n": ===

ssd1306_SetCursor(1, 1); // Coloca o cursor na posição

ssd1306_WriteStringWrapped("Um exemplo de texto com quebra automatica!\nE nova linha automaticamente!", Font_7x10, White);

```
É uma função para escrever texto com quebra automática de linha (ou não se escolher a função sem `Wrapped`). Se o texto ultrapassar a largura do display, o cursor é reposicionado para a próxima linha. Além disso, interpreta o caractere de nova linha (`\n`).

INSERIR IMG
______________
Outro exemplo com texto longo:
```c
while (1)

{

//=== Exibe em loop um texto que não cabe totalmente na tela fazendo o scroll horizontal (sem quebra de linha). ===

ssd1306_ScrollTextHorizontal(longText, Font_7x10, White, 10, 1);

}

return  0;
```
INSERIR IMG

 **Funções de Scroll (Rolagem):**
```c
//=== Inicia scroll horizontalmente da tela, automáticamente, da página 0 até 7 com velocidade definida (0 a 7, onde 0 é mais rápido(terceiro número)) ===

ssd1306_StartScrollLeft(0, 7, 0); // scroll da esquerda

//ssd1306_StartScrollRight(0, 7, 0); // scroll da direita

//=== Desenha um retângulo de (0, 0) a (127, 31) com a cor branca. Exemplo de uso como moldura, e etc. ===

ssd1306_DrawRectangle(0, 0, 127, 63, White);

//=== Desenha um triângulo com a cor branca. Exemplo de uso junto da moldura e/ou com outras formas geométricas, etc. (é sobreposto) ===

ssd1306_FillTriangle(10, 10, 50, 10, 30, 40, White); // triângulo preenchido
```
IMG SCROLL com formas




