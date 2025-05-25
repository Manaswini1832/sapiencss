import "./Error.css";

export const Error = (props) => {
    return <div className="errorMessageBox">
        {props.errorMessage !== "" ? <p>Error : {props.errorMessage}</p> : null}
    </div>
}