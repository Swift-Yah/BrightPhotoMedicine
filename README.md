![Bright | Photomedicine](http://www.brightmed.com.br/assets/images/site/logo1.png)
# BrightPhotoMedicine

## Desafio Bright

Primeiramente, parabéns por se juntarem a nós da Bright. Sou o Gabriel e vou passar algumas informações sobre o primeiro desafio de vocês.

### Desafio 1: controlar placas de LED usando o celular

Inicialmente terão à disposição os seguintes itens:

* Uma Arduino Uno com cabo USB
* Um módulo bluetooth
* Cabos diversos (fêmea/macho, macho/macho)
* Leds e resistores
* Componentes eletrônicos diversos
* [Relatório técnico que servirá como guia de desenvolvimento](https://onedrive.live.com/redir?resid=2D1AF576A0781255!397208&authkey=!AObcdfhlYZMEars&ithint=file%2cdocx)

**Justificativa:** Pacientes precisarão de doses corretas de luz para que o tratamento seja eficiente.

**Objetivo:** Com esse material vocês deverão desenvolver um protocolo de comunicação entre o Arduino em um [Chrome App](https://chrome.google.com/webstore/category/apps) – o navegador Google Chrome permite que você crie um aplicativo dentro dele e acesse como se fosse um programa. Com esse App é possível abrir uma conexão serial e enviar mensagens do seu computador para o módulo bluetooth e o Arduino.

**Produto gerado nesse desafio:** 2 placas de 25 LED e um Chrome App.

**Requisitos:**

1. As 2 placas devem funcionar quando ligadas ao protótipo do Light-Aid. <br />
2. O aplicativo deve controlar dois ou mais placas de leds com os seguintes parâmetros: intensidade,
frequência e tempo que fica ligado. Além disso, quando um led está ligado, você deve conseguir o interromper em qualquer momento (desligá-lo). Todos esses 3 valores o usuário deve conseguir modificar. Deve existir um botão, para cada led, chamado “iniciar” – quando clicado o led deve ficar acesso o tempo definido pelo usuário e obedecer aos outros parâmetros.
3. O app deve conseguir listar e selecionar as portas disponíveis. Se o usuário selecionar a porta errada, que não seja seu Arduino, o app deve alertar o usuário a cancelar a conexão. Se a conexão for estabelecida com sucesso, o seu Arduino deve mandar o nome dele para o app – gosto de nomes criativos, fiquem à vontade. Para simplificar a interface do usuário vamos estabelecer um baud-rate de 9600.

**Observações:**

Note que seu Arduino pode ter três leds e quando seu app se conectar a ele, deve aparecer a opção de controle dos três leds de maneira individual. Além disso, no código Arduino deve existir um vetor “leds” (pinos em que estão conectados os leds) e uma variável “nleds” (número de leds). Dessa maneira fica fácil adicionar e retirar novos leds.

O desafio parece um pouco complicado, e é para ser mesmo (LoL!!!). Para resolvê-lo vão precisar ter um pouco de conhecimento em Javascript e C. Não precisa se preocupar com a interface do usuário (por enquanto pode ser feia, mas tem que ter botões e inputs).

Não esqueça de pensar em situações como: o que acontece se perder a conexão bluetooth enquanto um led tiver ligado? Será que a Arduino recebeu o sinal que eu enviei?

Por favor, escrevam um código indentado, modularizado e fácil de ler. Adicione o mínimo de comentários possíveis, mas explique as funções. Um professor meu sempre diz que um programa é como um livro, portanto ele deve ser explicativo por si só, sem notas de rodapé. O resultado desse trabalho será utilizado por outros programadores e pessoas, leve isso em consideração antes de começar a programar em POG.

Parte do que vocês desenvolverão, será utilizada nos aplicativos oficiais da Bright. Portanto, documente como o seu protocolo de comunicação funciona e quais funções você criou. Especifique porque cada byte é enviado entre o app e o Arduino e para que ele serve.

**Material para consulta e apoio**
* (https://developer.chrome.com/apps/app_serial)
* (https://www.arduino.cc/en/Reference/Serial)
* (https://github.com/garrows/browser-serialport/tree/master/demo)

**URLs interessantes**
* (https://chrome.google.com/webstore/developer/dashboard)
* (https://developer.chrome.com/apps/chrome_apps_on_mobile)

Boa sorte.
Qualquer dúvida, pode mandar um e-mail gabriel@capella.pro
