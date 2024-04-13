/***
 * tabName is the id of the element to show
 */
let setDataSourceTab = function(evt, tabName)
{
    ///let tab = document.getElementById(tabName);
    //let tabs = tab.parentElement.children;
    let tabs = document.getElementById("select-data-source").children;

    for (i = 0; i < tabs.length; i++)
    {
        if(tabs[i].getAttribute("data-tab") === tabName)
        {
            tabs[i].classList.add("active");
        }
        else
        {
            tabs[i].classList.remove("active");
        }
    }

    let tabContents = document.getElementById("data-source-content").children;
    for (i = 0; i < tabContents.length; i++)
    {
        if(tabContents[i].getAttribute("data-tab") === tabName)
        {
            tabContents[i].style = "display: block";
        }
        else
        {
            tabContents[i].style = "display: none";
        }
    }
    
}


let setImageWidth = function(width)
{

}


let setImageHeight = function(height)
{

}


let toggleBigEndian = function()
{

}


let setOffset = function(offset)
{

}