

char* webPageHead() {
  char* head = "<!DOCTYPE html>\
               <head>\
               <meta charset = 'utf-8'>\
               <meta http-equiv = 'X-UA-Compatible' content = 'IE=edge'>\
               <title> Plantae Farm</title>\
               <meta name = 'viewport' content = 'width=device-width, initial-scale=1'>\
               <style>\
                form {display : flex;  flex-direction : column;  justify-content : center;}\
                .form-input {display:  flex;  flex-direction : column;  margin-bottom : 10px;}\
                .border_radius {  border-radius : 5px;border:#ccc 1px solid;}\
                .form-input input {  border-radius : 5px;border:#ccc 1px solid;}\
                .form-input label {  font-size : 15px;}\
                .button {  background-color : #1fa3ec;color:  white;height:  30px;  min-width : 200px;  margin-bottom : 10px;}\
                .partition {  margin-top : 10px;  margin-bottom : 10px;border:  solid 1px #ccc;}\
                .container {  align-items : center;  justify-content : center;display:  flex;  flex-direction : column;}\
</style></head> ";
  return head;
}
String webPageStatus(String statusWifi, String statusMqtt) {
  String status = "<p>Status WIFI: <b>" + statusWifi + "</b></p>\
            <p>Status MQTT: <b>"+ statusMqtt + "'</b></p>\
            <div class='partition'> </div>\
            <a href='http://192.168.4.1/'> <button class='button border_radius'> Refresh </button></a>\
            <a href='http://192.168.4.1/config'><button class='button border_radius'> Config </button></a>";

  return status;
}
char* webPageBody() {
  char* body = " <body><div class='container'>";
  return body;
}
char* webPageFooter() {
  char* footer = " </div></body></html>";
  return footer;
}
String webPageFormConfig(String ssid, String mqttServe, String port, String user) {
  String form = "<div class='partition'> </div>\
                 <form method='GET' action='/saveInfo'>\
                <div class='form-input'>\
                    <label for='wifi'>SSID:</label>\
                    <input type='text' id='wifi' name='wifi' value ='"+ ssid + "'>\
                </div>\
                <div class='form-input'>\
                    <label for='wifiPass'>PASSWORD:</label>\
                    <input type='text' id='wifiPassword' name='wifiPassword'\
                        value>\
                </div>\
                <div class='partition'> </div>\
                <div class='form-input'>\
                    <label for='mqttServe'>MQTT SERVER: </label>\
                    <input type='text' id='mqttServe' name='mqttServe' value ='"+ mqttServe + "' >\
                </div>\
                <div class='form-input'>\
                    <label for='mqttPort'> PORT: </label>\
                    <input type='text' id='mqttPort' name='mqttPort' value ='"+ port + "' >\
                </div>\
                <div class='form-input'>\
                    <label for='mqttUser'> USER: </label>\
                    <input type='text' id='mqttUser' name='mqttUser' value ='"+ user + "' >\
                </div>\
                <div class='form-input'>\
                    <label for='mqttPass'> PASSWORD: </label>\
                    <input type='text' id='mqttPassword' name='mqttPassword'\
                        value>\
                </div>\
                <div class='form-input'>\
                    <input type='text' id='session' name='session'\
                        style='display:none' value='123'>\
                </div>\
                <div class='form-input'>\
                    <input type='submit' value='Salvar'\
                        style='background-color:#1fa3ec' class='buttom_submit'>\
                </form>";
  return form;
}
char* webPageSave() {
  char* save = "<!DOCTYPE html><html><head><meta charset='utf-8'/><title>PlantaeFarm</title><meta http-equiv='refresh'content='2; URL= http://192.168.4.1/'/></head><body><h1>Informação salva com sucesso</h1></body></html>";
  return save;
}