const container=document.getElementById("content");

if(window.Worker) {
    const worker=new Worker("worker.js");
    container.innerHtml="Worker created, waiting for working..."
    worker.onmessage=(result => {
        const newElement=document.createElement("p");
        newElement.innerHtml=result;
        container.appendChild(newElement);
    })
}
else {
    container.innerHtml="Worker not supported";
}
