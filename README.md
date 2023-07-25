# ESP32-Gerenciador de wifi
Codigo para facilitar na seleção de redes para ESP32, sem auxilio de um computador, toda a conexão é feita por meio do acesso ao ESP no modo AP aonde você ira selecionar uma rede e conectar na mesma sem auxilio de cabos, o programa conta tambem com funções para configuração e conexão a um Broker MQTT.

Funcionamento:

Altera as variaveis ssidAP para alterar o nome da rede que o ESP ira criar e passwordAP para alterar a senha de acesso a esta rede
Grave o ESP32 e conecte a rede WIFI que ele gerou, com o dispositivo conectado va ate o navegador e digite o ip 192.168.4.1, ao realizar esta etapa você sera direcionado para a pagina principal


![image](https://github.com/Guilherme-Vilela/ESP32-GerenciadorWifi/assets/68655658/1445aaba-dae5-4da1-99db-445564c214de)


A pagina principal ira informar o estado da conexão do ESP com a rede wifi que você selecionar e o estado do broker MQTT, junto a isso a opção de recarregar a pagina para atualizar as informações e o botao de config que da acesso a pagina de configuração.


![image](https://github.com/Guilherme-Vilela/ESP32-GerenciadorWifi/assets/68655658/ebb67bb0-3f13-4959-9f38-454f267faed7)


Na pagina de configuração você pode selecionar a rede Wifi e inserir as suas credenciais com possibilidade tambem de inserir credenciais do broker mqtt, após inserir as credenciais clique em salvar, você sera redirecinado a pagina principal e pronto so precisa esperar o ESP conectar a sua rede WIFI, o status ira indicar quando ele realizar a conexão, mas se quiser obter mais informações conecte o ESP ao computador e verifique a sua serial.

