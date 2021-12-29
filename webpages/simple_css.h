#ifndef SIMPLE_CSS_H
#define SIMPLE_CSS_H

const char SIMPLE_CSS[] PROGMEM = R"=====(
    body {
        font-family: Arial, sans-serif;
        color: Black;
        padding: 1rem;
    }
    table {
        width: 70vw;
        font-family: 'Courier New', monospace;
        padding: 1rem 0;
    }
    table thead tr:last-child th {
        font-weight: bold;
        background-color: LightGrey;
        border-bottom: 1px solid LightGrey;
    }
    .table-sub-head {
        text-align:center;
        background-color: LightGrey;
    }
    td a {
        font-size: 0.8rem;
    }
    .button-symbol {
        width: 3rem;
        height: 3rem;
        font-size: 2rem;
        font-weight: bold;
    }
    .button-success {
        background-color: LightGreen;
    }
    .button-error {
        background-color: Tomato;
    }
    .info {
        text-align: right;
        border-top: 1px solid DarkGray;
        color: Gray;
        font-style: italic;
        font-size: small;
    }
    .title {
        border-top: 1px solid Black;
    }
    .navigation-links {
        display: flex;
        justify-content: flex-end;
    }
    .navigation-links div{
        padding: 0 1rem;
    }
)=====";

#endif