const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express')
const nodemailer = require("nodemailer");
const exphbs  = require('express-handlebars');
const port = 3000;

const app = express();