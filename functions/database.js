const admin=require('firebase-admin');

var serviceAccount = require('./serviceAccountKey.json');

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://meteo3d-d1068.firebaseio.com"
});

var db=admin.database();
var measurementRef = db.ref("measurements");

const userOperation={
    getMeasurements(res){
        measurementRef.once('value',function(snap){
            res.status(200).json({"measurements":snap.val()});
        })
    },
}

module.exports=userOperation;
