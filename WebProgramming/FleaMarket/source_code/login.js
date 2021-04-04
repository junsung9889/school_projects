var firebaseConfig = {
  apiKey: "AIzaSyCkJazvViVr2NMOk9dVEbHhVgnqLiji-oU",
  authDomain: "webproject-f6cf9.firebaseapp.com",
  databaseURL: "https://webproject-f6cf9-default-rtdb.firebaseio.com",
  projectId: "webproject-f6cf9",
  storageBucket: "webproject-f6cf9.appspot.com",
  messagingSenderId: "156486106438",
  appId: "1:156486106438:web:37897bb59ec1d80f953947",
  measurementId: "G-E888NHD3WH"
};
firebase.initializeApp(firebaseConfig);
var database = firebase.database();
var dbTestRef = database.ref('users/')
var users = []
dbTestRef.on('child_added',function(data){
  users.push({id: data.val().id, pw: data.val().pw,name:data.val().name,member:data.val().member})
})

const myStorage = window.localStorage;

function store() {
    var id = document.getElementById('user');
    var pw = document.getElementById('pass');
    var name = document.getElementById('name');
    var target = document.getElementById('member');
    var member = target.options[target.selectedIndex].text;
    var user = {id: id.value, pw: pw.value, name: name.value, member: member};
    if (name != null && pw != null){
      dbTestRef.push(user);
      users.push(user);
    }
    alert("Sign up Success");
}

function check() {
    var userId = document.getElementById('userName');
    var userPw = document.getElementById('userPw');
    for(key in users){
      if(userId.value == users[key].id && userPw.value == users[key].pw){
        alert("Log in Success");
        myStorage.setItem('login_user', JSON.stringify({id: users[key].id, pw: users[key].pw, name: users[key].name, member: users[key].member}));
        location.href = "index.html";
        return;
      }
    }
    alert('Log in Failed');
}
