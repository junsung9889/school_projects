const myStorage = window.localStorage;
var user = JSON.parse(myStorage.getItem('login_user'));
if(user){
  console.log(user);
  document.getElementById("login_user").innerHTML = "Hello! "+ user.member + user.name;
}

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
var dbTestRef = database.ref('users');
var dbTestRef1 = database.ref('products');
var i = 0;

dbTestRef.on('child_added',function(data){
  if(data){
    document.getElementsByTagName("td")[i+1].innerHTML = data.val().id;
    document.getElementsByTagName("td")[i+2].innerHTML = data.val().name;
    document.getElementsByTagName("td")[i+3].innerHTML = data.val().member;
    i+=6;
  }
});

function modifyMemberType(num){
  var updates = {};
  var target = document.getElementsByTagName("select")[num];
  var member = target.options[target.selectedIndex].text;
  var j =0;
  dbTestRef.on('child_added',function(data){
    if(j == num){
      if(member != "Choose..."){
        updates['member'] = member;
        dbTestRef.child(data.key).update(updates);
        window.alert("Update Member Success!");
      }
    }
    j++;
  });
  location.reload();
}
function deleteMember(num){
  var j =0;
  var ukey = "";
  dbTestRef.on('child_added',function(data){
    if(j == num){
      ukey = data.key;
    }
    j++;
  });
  dbTestRef.child(ukey).remove();
  location.reload();
}
