
#ifndef GAUGE_HTML_H
#define GAUGE_HTML_H

const char GAUGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Gauge - Temperature &amp; humidity reporter</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="content-type" content="text/html;charset=UTF-8">
    <link rel="stylesheet" href="/styleSimple_css">
</head>
<body onload="javascript:start();">
    <h1 class="title">Gauge - Temperature &amp; humidity reporter</h1>
    <h2>Software</h2>
    <p>This software is intended for Arduino and compatible PLCs. The purpose is to use the PLC with.</p>
    <h3>Device</h3>
    
    <nav class="navigation-links"><div><a data-bind="attr: { href: computed.href.gauge }">Gauge </a></div><div>|</div><div><a data-bind="attr: { href: computed.href.info }">Device info</a></div></nav>
    <div class="info">
        <p>This software is distributed under <a href = "https://en.wikipedia.org/wiki/MIT_License">MIT License</a>. Source code on <a href="https://github.com/HaunsTM">Github - HaunsTM</a></p>
    </div>
    

    <script>
        var websock;
        function start() {
            websock = new WebSocket('ws://' + window.location.hostname + ':81/');
            websock.onopen = function(evt) { console.log('websock open'); };
            websock.onclose = function(evt) { console.log('websock close'); };
            websock.onerror = function(evt) { console.log(evt); };
            websock.onmessage = function(evt) {
                console.log(evt);
            }
            function buttonclick(e) {
            websock.send(e.id);
            }
        }
    </script>
    <script src="/javascriptKnockout_js">
    </script>

    <script src="/constJavascriptParameters_js">
    </script>

    <script>
        function viewModelKnockout() {
            const _self = this;
            
            _self.device = {
                firmwareVersion: ko.observable(constJavascriptParameters.device.firmwareVersion),
                macAddress: ko.observable(constJavascriptParameters.device.macAddress),
                serialMonitorBaud: ko.observable(constJavascriptParameters.device.serialMonitorBaud),
            };
            _self.mqtt = {
                publishTopics: {
                    temperatureC: ko.observable(constJavascriptParameters.mqtt.publishTopics.temperatureC),
                    humidityPercent: ko.observable(constJavascriptParameters.mqtt.publishTopics.humidityPercent),
                },
                hostname: ko.observable(constJavascriptParameters.mqtt.hostname),
                clientId: ko.observable(constJavascriptParameters.mqtt.clientId),
                connectionOptions: {
                    userName: ko.observable(constJavascriptParameters.mqtt.connectionOptions.userName),
                    password: ko.observable(constJavascriptParameters.mqtt.connectionOptions.password),
                    keepAliveInterval: ko.observable(constJavascriptParameters.mqtt.connectionOptions.keepAliveInterval),
                },
                port: ko.observable(constJavascriptParameters.mqtt.port)
            };
            _self.wifi = {
                channel: ko.observable(constJavascriptParameters.wifi.channel),
                localIP: ko.observable(constJavascriptParameters.wifi.localIP),
                SSID: ko.observable(constJavascriptParameters.wifi.SSID),
            };
            _self.computed = (function () {                
                return {
                    href: {
                        base: ko.pureComputed( () => { return "//" + _self.wifi.localIP() + "/" ; }),
                        gauge: ko.pureComputed( () => { return "//" + _self.wifi.localIP() + "/gauge" }),
                        info: ko.pureComputed( () => { return "//" + _self.wifi.localIP() + "/info" }),
                        sensorData: ko.pureComputed( () => { return "//" + _self.wifi.localIP() + "/sensorData" }),
                    }
                }
            })();
            return _self;
        };
        ko.applyBindings(viewModelKnockout);
    </script>
</body>
</html>
)=====";

#endif