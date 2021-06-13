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
  storageBucket: "gs://webproject-f6cf9.appspot.com",
  messagingSenderId: "156486106438",
  appId: "1:156486106438:web:37897bb59ec1d80f953947",
  measurementId: "G-E888NHD3WH"
};
firebase.initializeApp(firebaseConfig);
var database = firebase.database();
var storageRef = firebase.storage().ref();
let selectedFile;

function registerProduct(){
    var dbTestRef = database.ref('products');
    var pname = document.getElementById("name");
    var pprice = document.getElementById("price");
    var pplace = document.getElementById("place");
    var pphone = document.getElementById("phone");
    var pstatus = document.getElementById("status");
    if(pname.value && pprice.value && pplace.value && pphone.value && pstatus.value){
      dbTestRef.push({"seller":user.id,"name":pname.value,"phone":pphone.value,"place":pplace.value,"price":pprice.value,"status":pstatus.value,"liked":0});
      dbTestRef.once('value', function(data){ storeImage(pname);});
    }
    else{
      window.alert("Fill all the blanks");
    }
}

function storeImage(pname){
  console.log(pname.value);
  storageRef.child('images/'+ pname.value).put(selectedFile).then(function(snapshot){
    console.log(snapshot);
    window.alert("Register Completed!");
    location.href = "register.html";
  });
}

function upload(){
  document.getElementById('fileInput').addEventListener('change', e => {
      selectedFile = e.target.files[0];
      console.log(e);
  });
  var selFile = document.querySelector('#fileInput');
  selFile.onchange = function () {
      var getList = this.files;
      // 읽기
      var reader = new FileReader();
      reader.readAsDataURL(getList[0]);

      //로드 한 후
      reader.onload = function () {
          document.querySelector('#photoFrame').src = reader.result ;
      };
  };
}
