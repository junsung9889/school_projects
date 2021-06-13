const myStorage = window.localStorage;
var user = JSON.parse(myStorage.getItem('login_user'));
if(user){
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
var dbTestRef = database.ref(user.id + '_wish');

let i = 0;
dbTestRef.on('child_added',function(data){
  if(data){
    document.getElementsByTagName("h6")[i].innerHTML = data.val().name;
    document.getElementsByTagName("span")[i+2].innerHTML = data.val().price;
  }
  i+=1;
});
setTimeout(() => {
        document.getElementById("total_num").innerHTML = i;
    }, 1000);
var dbTestRef1 = database.ref();

function addCart(num){
  var i = 0;
  dbTestRef.on('child_added',function(data){
    if(i == num){
      dbTestRef1.child(user.id+"_cart").push({name:data.val().name,price:data.val().price});
      window.alert("Add to Cart Success!");
    }
    i++;
  });
}

function removeWish(){
  dbTestRef.remove();
  window.alert("Remove Wish Completed!");
}
