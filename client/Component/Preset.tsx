import React from 'react';

import useActivate from '#/Hooks/PresetActivate';

/**
 * Preset View Properties
 */
export type PresetProps = {
    /** title of preset */
    title: string;
    /** key of preset */
    name: string;
};

/**
 * Preset View
 * @param props properties 
 */
const Component: React.FC<PresetProps> = (props: PresetProps) => {
    const [active, activate] = useActivate(props.name);
    const className = active ? 'active' : '';

    return (
        <button onClick={activate} className={className}>
            <h2>{props.title}</h2>
        </button>
    );
}

export default Component;
