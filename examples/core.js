console.log('hello from core', core.number());

core.onMessage((sender, msg) => console.log('core', core.number(), 'received', msg, 'from core', sender));

time.sleep(1).then(() => {
    if (core.number() == 0) {
        core.sendToAll('hello broadcast');
    }
});
