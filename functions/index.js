const functions = require('firebase-functions');
const firebase = require('firebase-admin');
const express = require('express');
const engines = require('consolidate');
//const nodemailer = require('nodemailer');
//const port = 3000;

const firebaseApp = firebase.initializeApp(
	functions.config().firebase
);

const app = express();
app.engine('hbs', engines.handlebars);
app.set('views', './views');
app.set('view engine', 'hbs');

app.use('/static',express.static('public'));

app.get('/', (request, response) => {
	response.set('Cache-Control', 'public, max-age=300, s-maxage=600');
		response.render('index', { text: 'handlebarsy też działają'});
});

app.get('/data.json', (request, response) => {
	const dbRef = firebase.database().ref()
	var data = dbRef.once('value').then(function(dataSnapshot) {
		return dataSnapshot.val();
	  });
	response.json(data);
});


/*
app.static(path.join(__dirname + "public"))
app.get('/', (req, res) => {
  res.sendFile( '/public/views/index.html'));
});

app.use('/', router);
app.listen(process.env.port || 3000);
*/
exports.app = functions.https.onRequest(app);
