import { useState } from 'react';

import { provideSingletonApi } from '#/Provider/IrClientProvider';

const api = provideSingletonApi();

const useActivate = (name: string): [boolean, () => Promise<void>] => {
    const [active, setActive] = useState<boolean>(false);

    const activate = async () => {
        await api.emit_by_preset_name(name);
        setActive(true);
        await new Promise(resolve => setTimeout(resolve, 500));
        setActive(false);
    };

    return [active, activate];
};

export default useActivate;
