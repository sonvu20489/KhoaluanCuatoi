document.addEventListener("DOMContentLoaded", ()=>{
    var a = document.getElementsByClassName('btnshow');
    var check=[1,1,1];

    a[0].addEventListener("click",()=>{
        if(check[0] == 1)
        {
            a[0].innerHTML = '<i class="fas fa-eye-slash"></i>';
            document.getElementById("input-password").setAttribute("type","text");
            check[0] = !check[0];
        }
        else
        {
            a[0].innerHTML = '<i class="far fa-eye"></i>';
            document.getElementById("input-password").setAttribute("type","password");
            check[0] = !check[0];   
        }
        

    })
    a[1].addEventListener("click",()=>{
        if(check[1] == 1)
        {
            a[1].innerHTML = '<i class="fas fa-eye-slash"></i>';
            document.getElementById("input-repass").setAttribute("type","text");
            check[1] = !check[1];
        }
        else
        {
            a[1].innerHTML = '<i class="far fa-eye"></i>';
            document.getElementById("input-repass").setAttribute("type","password");
            check[1] = !check[1];   
        }
        

    })
    a[2].addEventListener("click",()=>{
        if(check[2] == 1)
        {
            a[2].innerHTML = '<i class="fas fa-eye-slash"></i>';
            document.getElementById("input-idprod").setAttribute("type","text");
            check[2] = !check[2];
        }
        else
        {
            a[2].innerHTML = '<i class="far fa-eye"></i>';
            document.getElementById("input-idprod").setAttribute("type","password");
            check[2] = !check[2];   
        }
        

    })
    var btn_toggle = document.querySelector('.symbol_menu');
    var menu = document.querySelector('.menu');
    var toggle = 0;
    btn_toggle.addEventListener("click",()=>{
        if(toggle == 0)
        {
            document.querySelector(".symbol_menu").innerText = '╳';
            document.querySelector(".symbol_menu").setAttribute('style','font-size:18px');
            menu.setAttribute('style','display:flex;');
            toggle = !toggle;
        }
        else
        {
            document.querySelector(".symbol_menu").setAttribute('style','');
            document.querySelector(".symbol_menu").innerText = '≡';
            menu.setAttribute("style",'display:none');
            toggle = !toggle;
        }
    })
})
