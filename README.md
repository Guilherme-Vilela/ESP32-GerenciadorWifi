# ESP32-Gerenciador de wifi
Codigo para facilitar na seleção de redes para ESP32, sem auxilio de um computador, toda a conexão é feita por meio do acesso ao ESP no modo AP aonde você irá selecionar uma rede e conectar na mesma sem auxílio de cabos, o programa conta também com funções para configuração e conexão a um Broker MQTT.

#Funcionamento:

Altera as variaveis ssidAP para alterar o nome da rede que o ESP irá criar e passwordAP para alterar a senha de acesso a esta rede
Grave o ESP32 e conecte a rede WIFI que ele gerou, com o dispositivo conectado vá até o navegador e digite o ip 192.168.4.1, ao realizar esta etapa você será direcionado para a página principal


![image](https://github.com/Guilherme-Vilela/ESP32-GerenciadorWifi/assets/68655658/1445aaba-dae5-4da1-99db-445564c214de)


A página principal irá informar o estado da conexão do ESP com a rede wifi que você selecionar e o estado do broker MQTT, junto a isso a opção de recarregar a página para atualizar as informações e o botão de config que dá acesso a página de configuração.


![image](https://github.com/Guilherme-Vilela/ESP32-GerenciadorWifi/assets/68655658/ebb67bb0-3f13-4959-9f38-454f267faed7)


Na página de configuração você pode selecionar a rede Wifi e inserir as suas credenciais com possibilidade também de inserir credenciais do broker mqtt, após inserir as credenciais clique em salvar, você será redirecionado à página principal e pronto só precisa esperar o ESP conectar a sua rede WIFI, o status irá indicar quando ele realizar a conexão, mas se quiser obter mais informações conecte o ESP ao computador e verifique a sua serial.
