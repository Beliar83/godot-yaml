pub(crate) struct GCSResult<T: Clone> {
    pub(crate) result: Option<T>,
    pub(crate) error: String,
}

impl<T: Clone> GCSResult<T> {
    pub(crate) fn new_result(result: T) -> Self {
        let result = Some(result);
        Self {
            result,
            error: String::default(),
        }
    }

    pub(crate) fn new_error(error: String) -> Self {
        Self {
            result: None,
            error,
        }
    }

    pub(crate) fn is_error(&self) -> bool {
        match self.result {
            None => true,
            Some(_) => false,
        }
    }

    pub(crate) fn get_result(&self) -> T {
        match &self.result {
            None => {
                panic!("Attempted to access result when is_error() is true")
            }
            Some(result) => result.clone(),
        }
    }

    pub(crate) fn get_error(&self) -> String {
        self.error.clone()
    }
}
