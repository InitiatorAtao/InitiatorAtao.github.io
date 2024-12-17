const params_template={
    uuid: "initiator"
};

async function getFeed(page) {
    const params={page:page,...params_template};
    return fetch(`https://api.nmb.best/api/feed?${new URLSearchParams(params)}`,{
    })
        .then(response => {
        if(!response.ok)
            throw new Error(`HTTP error ${response.status}`);
        return response.json();
        })
}

async function scanFeed(callBack) {
    let page=1;
    for(;;++page) {
        const result=await getFeed(page)
        if(result.length>0)
            callBack(result);
        else break;
    }
}

scanFeed(result => {
    console.log(result);
})
