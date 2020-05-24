const functions = require('firebase-functions');
const firebase = require('firebase-admin');
const express = require('express');
const engines = require('consolidate');
const bodyParser = require('body-parser');
const userOperation =require('./database');
const nodemailer = require('nodemailer');

const app = express();
app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());

app.engine('hbs', engines.handlebars);
app.set('views', './views');
app.set('view engine', 'hbs');


app.get('/data',(req,res)=>{
    userOperation.getMeasurements(res);
})

app.use('/static', express.static('public'));

app.get('/', (request, response) => {
    response.set('Cache-Control', 'public, max-age=300, s-maxage=600');
    response.render('index', {
        text: "METEO 3D API"
    });
});

exports.app = functions.https.onRequest(app);
