const fs = require("fs");

function cmp(a, b) {
    if (typeof a === "number" && typeof b === "number") {
        return a-b;
    }

    if (typeof a === "number") a = [a];
    if (typeof b === "number") b = [b];

    for (let i = 0; i < Math.min(a.length, b.length); i++) {
        let res = cmp(a[i], b[i]);
        if (res !== 0) return res;
    }

    return a.length - b.length;
}

function f(s) {
    let a = s.split("\n\n").map(g => g.split("\n").map(v => eval(v)));
    let i = 0;
    let sum = 0;
    for (const g of a) {
        i++;
        const res = cmp(g[0], g[1]) < 0;
        if (res) sum += i;
    }
    return sum;
}

function g(s) {
    s = s.replace(/\n{2}/g, '\n');
    const a = s.split("\n").map(v => [...eval(v)/*.flat(999)*/.values()]);
    a.push([2]);
    a.push([6]);

    const sa = a.sort(cmp);

    let j = sa.findIndex(v => v.length === 1 && v[0] === 2) + 1;
    let k = sa.findIndex(v => v.length === 1 && v[0] === 6) + 1;

    console.log(j, k);
    return j*k;
}

fs.readFile(process.argv[2], 'utf8', function (err, data) {
    if (err) {
        return console.log(err);
    }
    //console.log(f(data));
    console.log(g(data));
});
