
#ifndef INFO_HTML_H
#define INFO_HTML_H

const char INFO_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Device info - Temperature &amp; humidity reporter</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="content-type" content="text/html;charset=UTF-8">
    <link rel="stylesheet" href="/styleSimple_css">
</head>
<body>
    <h1 class="title">Device info - Temperature &amp; humidity reporter</h1>
    <h2>Software</h2>
    <p>This software is intended for Arduino and compatible PLCs. The purpose is to use the PLC with.</p>
    <h3>Device</h3>
    <table>
        <thead>
            <tr>
                <th>Parameter</th>
                <th>Value</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Firmware version</td>
                <td><span data-bind="text: device.firmwareVersion"></span></td>
            </tr>        
            <tr>
                <td>MAC address</td>
                <td><span data-bind="text: device.macAddress"></span></td>
            </tr>
            <tr>
                <td>Serial monitor communication speed</td>
                <td><span data-bind="text: device.serialMonitorBaud"></span></td>
            </tr>
        </tbody>
    </table>
    <h4>Sensor</h4>
    <table>
        <thead>
            <tr>
                <th>Physical quantity</th>
                <th>Range</th>
                <th>Tolerance</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Temperature</td>
                <td><code>-40 to 80 &deg;C</code></td>
                <td><code>&plusmn;0.5 %</code></td>
            </tr>
            <tr>
                <td>Humidity</td>
                <td><code> 0 to 100 %RH</code></td>
                <td><code>&plusmn;2 %RH</code></td>
            </tr>
        </tbody>
    </table>
    <h3>Wifi</h3>
    <table>
        <thead>
            <tr>
                <th>Parameter</th>
                <th>Value</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Channel</td>
                <td><span data-bind="text: wifi.channel"></span></td>
            </tr>
            <tr>
                <td>IP</td>
                <td><span data-bind="text: wifi.localIP"></span></td>
            </tr>
            <tr>
                <td>SSID</td>
                <td><span data-bind="text: wifi.SSID"></span></td>
            </tr>
        </tbody>
    </table>
    <h3>HTTP requests</h3>
    <h4>Sensor data</h4>
    <table>
        <thead>
            <tr>
                <th>HTTP request</th>
                <th>Expected response</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>
                    <a data-bind="attr: { href: computed.href.sensorData }"><span data-bind="text: computed.href.sensorData" /></a>
                        <br /><br />
                    Get current read measurement from sensor in a JSON:<br />
                </td>
                <td>
<pre>{
    "temperatureCelcius":"xxx",
    "humidityPercent":"yyy",
}</pre>
                </td>
            </tr>
        </tbody>
    </table>
    <h3>MQTT</h3>
    <h4>Settings</h4>
    <table>
        <thead>
            <tr>
                <th></th>
                <th>Value</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Host</td>
                <td><span data-bind="text: mqtt.hostname"></span></td>
            </tr>
            <tr>
                <td>Port</td>
                <td><span data-bind="text: mqtt.port"></span></td>
            </tr>
            <tr>
                <td>Username</td>
                <td><span data-bind="text: mqtt.connectionOptions.userName"></span></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><span data-bind="text: mqtt.connectionOptions.password"></span></td>
            </tr>
        </tbody>
    </table>
    <h4>Topics</h4>
    <table>
        <thead>
            <tr>
                <th>Physical quantity</th>
                <th>Topic</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Temperature</td>
                <td><code><span data-bind="text: mqtt.publishTopics.temperatureC"></span></code></td>
            </tr>
            <tr>
                <td>Humidity</td>
                <td><code><span data-bind="text: mqtt.publishTopics.humidityPercent"></span></code></td>
            </tr>
        </tbody>
    </table>
    <nav class="navigation-links"><div><a data-bind="attr: { href: computed.href.gauge }">Gauge </a></div><div>|</div><div><a data-bind="attr: { href: computed.href.info }" style="text-decoration: none;">Device info</a></div></nav>
    <div class="info">
        <p>This software is distributed under <a href = "https://en.wikipedia.org/wiki/MIT_License">MIT License</a>. Source code on <a href="https://github.com/HaunsTM">Github - HaunsTM</a></p>
    </div>
    
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