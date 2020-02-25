const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express')
const nodemailer = require('nodemailer');
const path = require('path');
const port = 3000;

const router = express.Router();
const app = express();

app.get('/', (req, res) => {
  res.sendFile( './../public/views/index.html'));
});

app.use('/', router);
app.listen(process.env.port || 3000);
exports.app = functions.https.onRequest(app);