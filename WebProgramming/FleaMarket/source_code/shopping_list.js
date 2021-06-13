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
var dbTestRef = database.ref(user.id + '_cart');

let i = 0;
let total_price = 0;
let names = [];
dbTestRef.on('child_added',function(data){
  if(data){
    document.getElementsByTagName("h6")[i].innerHTML = data.val().name;
    document.getElementsByTagName("span")[i+2].innerHTML = data.val().price;
    total_price += parseInt(data.val().price.substring(1,));
    names.push(data.val().name);
  }
  i+=1;
});
total_price -= 50;
setTimeout(() => {
        document.getElementById("total_num").innerHTML = i;
        document.getElementsByTagName("strong")[0].innerHTML = "$" + total_price;
    }, 1000);

var dbTestRef1 = database.ref('products');

function buyProduct(){
  dbTestRef1.on('child_added',function(data){
    for (k in names){
    if(data.val().name == names[k]){
      var updates = {};
      updates['status'] = "Purchased";
      dbTestRef1.child(data.key).update(updates);
      }
    }
  });
  dbTestRef.remove();
  window.alert("Buy Completed!")
}

function removeCart(){
  dbTestRef.remove();
  window.alert("Remove Cart Completed!");
}
